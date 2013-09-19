/****************************************************************************************
 * Copyright (c) 2010 Sergey Ivanov <123kash@gmail.com>                                 *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#define DEBUG_PREFIX "MusicDNSAudioDecoder"

#include "MusicDNSAudioDecoder.h"

#include <config.h>
#include "core/support/Debug.h"
#include "core/meta/Meta.h"

extern "C" {
    typedef quint64 UINT64_C;
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/mathematics.h>
}

#include <ofa1/ofa.h>

DecodedAudioData::DecodedAudioData()
                 : m_sRate( 0 )
                 , m_channels( 0 )
                 , m_duration( 0 )
                 , m_data( new QByteArray )
{
}

DecodedAudioData::~DecodedAudioData()
{
    if( m_data )
        delete m_data;
}

int
DecodedAudioData::sRate()
{
    return m_sRate;
}

quint8
DecodedAudioData::channels()
{
    return m_channels;
}

int
DecodedAudioData::setupInitial( const int sampleRate, const quint8 channels )
{
    m_sRate = sampleRate;
    m_channels = channels;
    return 0;
}

const char *
DecodedAudioData::data()
{
    return m_data->data();
}

qint64
DecodedAudioData::duration()
{
    return m_duration;
}

void
DecodedAudioData::addTime( const qint64 ms )
{
    m_duration += ms;
}

int
DecodedAudioData::length()
{
    return m_data->length();
}

int
DecodedAudioData::appendData( const quint8 *data, int length )
{
    m_data->append( (const char *)data, length );
    return 0;
}

DecodedAudioData &DecodedAudioData::operator<< ( const quint8 &byte )
{
    m_data->append( byte );
    return *this;
}

void DecodedAudioData::flush()
{
    m_sRate = 0;
    m_channels = 0;
    m_duration = 0;
    m_data->clear();
}

MusicDNSAudioDecoder::MusicDNSAudioDecoder( const Meta::TrackList &tracks, const int sampleLength )
                    : Job()
                    , m_tracks( tracks )
                    , m_sampleLength( sampleLength )
{
}

MusicDNSAudioDecoder::~MusicDNSAudioDecoder()
{
}

void
MusicDNSAudioDecoder::run()
{
    DecodedAudioData data;

    avcodec_register_all();
    av_register_all();

    foreach( Meta::TrackPtr track, m_tracks )
    {
        decode( track->playableUrl().toLocalFile(), &data, m_sampleLength );
        if( data.duration() > MIN_SAMPLE_LENGTH )
        {
            QString fingerprint( ofa_create_print( ( unsigned char * ) data.data(),
                                                   OFA_LITTLE_ENDIAN, ( data.length() >> 1 ),
                                                   data.sRate(), data.channels() ) );
            emit trackDecoded( track, fingerprint );
        }
        else
            warning() << QLatin1String( "Some error occurred during fingerprint generation, probably track is too short: " ) +
                         track->playableUrl().toLocalFile();
        data.flush();
    }
}

// Function below has separate implementation for each ffmpeg API version
int
MusicDNSAudioDecoder::decode( const QString &fileName, DecodedAudioData *data, const int length )
#if LIBAVCODEC_VERSION_MAJOR >= 54  // ffmpeg 0.11
{
    AVFormatContext *pFormatCtx = NULL;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;
    AVFrame *decodedFrame = NULL;
    AVPacket packet, avpkt;
    AVRational streamTimeBase = { 1, 1000000 };
    AVRational localTimeBase = { 1, 1000 };

    int audioStream = 0;
    int gotFrame = 0;
    int decoderRet = 0;
    int planeSize = 0;

    bool isOk = true;

    if( avformat_open_input( &pFormatCtx, fileName.toLocal8Bit(), NULL, NULL ) < 0 )
    {
        warning() << QLatin1String( "Unable to open input file: " ) + fileName;
        return 0;
    }

    if( avformat_find_stream_info( pFormatCtx, NULL ) < 0 )
    {
        warning() << QLatin1String( "Unable to find stream info: " ) + fileName;
        avformat_close_input( &pFormatCtx );
        return 0;
    }

    audioStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &pCodec, 0);
    if( audioStream < 0 )
    {
        warning() << QLatin1String( "Unable to find stream: " ) + fileName;
        avformat_close_input( &pFormatCtx );
        return 0;
    }

    if( !pCodec )
    {
        warning() << QLatin1String( "Unable to find decoder: " ) + fileName;
        avformat_close_input( &pFormatCtx );
        return 0;
    }

    pCodecCtx = pFormatCtx->streams[audioStream]->codec;

    if( avcodec_open2( pCodecCtx, pCodec, NULL ) < 0 )
    {
        warning() << QLatin1String( "Unable to open codec " ) + fileName;
        avformat_close_input( &pFormatCtx );
        return 0;
    }

    streamTimeBase = pFormatCtx->streams[audioStream]->time_base;

    if( data->setupInitial( pCodecCtx->sample_rate,
                            ( pCodecCtx->channels > 1 )? 1 : 0 ) )
        return 0;

    av_init_packet( &avpkt );
    while( !av_read_frame( pFormatCtx, &packet ) && isOk )
    {
        if( packet.stream_index == audioStream )
        {
            avpkt.size = packet.size;
            avpkt.data = packet.data;
            while( avpkt.size > 0 )
            {
                if( !decodedFrame )
                {
                    decodedFrame = avcodec_alloc_frame();
                    if( !decodedFrame )
                    {
                        warning() << "Unable to allocate enough memory to decode file.";
                        isOk = false;
                        break;
                    }
                    else
                        avcodec_get_frame_defaults( decodedFrame );
                }

                decoderRet = avcodec_decode_audio4( pCodecCtx, decodedFrame, &gotFrame, &avpkt );
                if( decoderRet < 0 )
                {
                    warning() << "Error while decoding.";
                    isOk = false;
                    break;
                }

                if( gotFrame )
                {

                    av_samples_get_buffer_size( &planeSize, pCodecCtx->channels, decodedFrame->nb_samples, pCodecCtx->sample_fmt, 1);
                    for( int i = 0; i < qMin( pCodecCtx->channels, 2 ); i++ )
                        if( data->appendData( const_cast<const quint8 *>( decodedFrame->extended_data[i] ), planeSize ) )
                        {
                            isOk = false;
                            break;
                        }
                }

                avpkt.size -= decoderRet;
                avpkt.data += decoderRet;
            }

            data->addTime( av_rescale_q( packet.duration, streamTimeBase, localTimeBase ) );
        }

        av_free_packet( &packet );

        if( data->duration() >= length )
            break;
    }

    av_free_packet( &avpkt );

    avcodec_close( pCodecCtx );
    avformat_close_input( &pFormatCtx );
    av_free( decodedFrame );

    return data->duration();
}
#elif LIBAVCODEC_VERSION_MAJOR == 53 && LIBAVCODEC_VERSION_MINOR >= 60  // ffmpeg 0.10
{
    AVFormatContext *pFormatCtx = NULL;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;
    AVFrame *decodedFrame = NULL;
    AVPacket packet, avpkt;
    AVRational streamTimeBase = { 1, 1000000 };
    AVRational localTimeBase = { 1, 1000 };

    int audioStream = 0;
    int gotFrame = 0;
    int decoderRet = 0;
    int planeSize = 0;

    bool isOk = true;

    if( avformat_open_input( &pFormatCtx, fileName.toLocal8Bit(), NULL, NULL ) < 0 )
    {
        warning() << QLatin1String( "Unable to open input file: " ) + fileName;
        return 0;
    }

    if( avformat_find_stream_info( pFormatCtx, NULL ) < 0 )
    {
        warning() << QLatin1String( "Unable to find stream info: " ) + fileName;
        avformat_close_input( &pFormatCtx );
        return 0;
    }

    audioStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &pCodec, 0);
    if( audioStream < 0 )
    {
        warning() << QLatin1String( "Unable to find stream: " ) + fileName;
        avformat_close_input( &pFormatCtx );
        return 0;
    }

    if( !pCodec )
    {
        warning() << QLatin1String( "Unable to find decoder: " ) + fileName;
        avformat_close_input( &pFormatCtx );
        return 0;
    }

    pCodecCtx = pFormatCtx->streams[audioStream]->codec;

    if( avcodec_open2( pCodecCtx, pCodec, NULL ) < 0 )
    {
        warning() << QLatin1String( "Unable to open codec " ) + fileName;
        avformat_close_input( &pFormatCtx );
        return 0;
    }

    streamTimeBase = pFormatCtx->streams[audioStream]->time_base;

    if( data->setupInitial( pCodecCtx->sample_rate,
                            ( pCodecCtx->channels > 1 )? 1 : 0 ) )
        return 0;

    av_init_packet( &avpkt );
    while( !av_read_frame( pFormatCtx, &packet ) && isOk )
    {
        if( packet.stream_index == audioStream )
        {
            avpkt.size = packet.size;
            avpkt.data = packet.data;
            while( avpkt.size > 0 )
            {
                if( !decodedFrame )
                {
                    decodedFrame = avcodec_alloc_frame();
                    if( !decodedFrame )
                    {
                        warning() << "Unable to allocate enough memory to decode file.";
                        isOk = false;
                        break;
                    }
                    else
                        avcodec_get_frame_defaults( decodedFrame );
                }

                decoderRet = avcodec_decode_audio4( pCodecCtx, decodedFrame, &gotFrame, &avpkt );
                if( decoderRet < 0 )
                {
                    warning() << "Error while decoding.";
                    isOk = false;
                    break;
                }

                if( gotFrame )
                {
                    
                    av_samples_get_buffer_size( &planeSize, pCodecCtx->channels, decodedFrame->nb_samples, pCodecCtx->sample_fmt, 1);
                    for( int i = 0; i < qMin( pCodecCtx->channels, 2 ); i++ )
                        if( data->appendData( const_cast<const quint8 *>( decodedFrame->extended_data[i] ), planeSize ) )
                        {
                            isOk = false;
                            break;
                        }
                }

                avpkt.size -= decoderRet;
                avpkt.data += decoderRet;
            }

            data->addTime( av_rescale_q( packet.duration, streamTimeBase, localTimeBase ) );
        }

        av_free_packet( &packet );

        if( data->duration() >= length )
            break;
    }

    av_free_packet( &avpkt );

    avcodec_close( pCodecCtx );
    avformat_close_input( &pFormatCtx );
    av_free( decodedFrame );

    return data->duration();
}
#elif LIBAVCODEC_VERSION_MAJOR == 53  && LIBAVCODEC_VERSION_MINOR >= 42 // ffmpeg 0.9
{
    AVFormatContext *pFormatCtx = NULL;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;
    AVFrame *decodedFrame = NULL;
    AVPacket packet, avpkt;
    AVRational streamTimeBase = { 1, 1000000 };
    AVRational localTimeBase = { 1, 1000 };

    int audioStream = 0;
    int gotFrame = 0;
    int decoderRet = 0;
    int planeSize = 0;

    bool isOk = true;

    if( avformat_open_input( &pFormatCtx, fileName.toLocal8Bit(), NULL, NULL ) < 0 )
    {
        warning() << QLatin1String( "Unable to open input file: " ) + fileName;
        return 0;
    }

    if( avformat_find_stream_info( pFormatCtx, NULL ) < 0 )
    {
        warning() << QLatin1String( "Unable to find stream info: " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    audioStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &pCodec, 0);
    if( audioStream < 0 )
    {
        warning() << QLatin1String( "Unable to find stream: " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    if( !pCodec )
    {
        warning() << QLatin1String( "Unable to find decoder: " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    pCodecCtx = pFormatCtx->streams[audioStream]->codec;

    if( avcodec_open2( pCodecCtx, pCodec, NULL ) < 0 )
    {
        warning() << QLatin1String( "Unable to open codec " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    streamTimeBase = pFormatCtx->streams[audioStream]->time_base;

    if( data->setupInitial( pCodecCtx->sample_rate,
                            ( pCodecCtx->channels > 1 )? 1 : 0 ) )
        return 0;

    av_init_packet( &avpkt );
    while( !av_read_frame( pFormatCtx, &packet ) && isOk )
    {
        if( packet.stream_index == audioStream )
        {
            avpkt.size = packet.size;
            avpkt.data = packet.data;
            while( avpkt.size > 0 )
            {
                if( !decodedFrame )
                {
                    decodedFrame = avcodec_alloc_frame();
                    if( !decodedFrame )
                    {
                        warning() << "Unable to allocate enough memory to decode file.";
                        isOk = false;
                        break;
                    }
                    else
                        avcodec_get_frame_defaults( decodedFrame );
                }

                decoderRet = avcodec_decode_audio4( pCodecCtx, decodedFrame, &gotFrame, &avpkt );
                if( decoderRet < 0 )
                {
                    warning() << "Error while decoding.";
                    isOk = false;
                    break;
                }

                if( gotFrame )
                {

                    av_samples_get_buffer_size( &planeSize, pCodecCtx->channels, decodedFrame->nb_samples, pCodecCtx->sample_fmt, 1);
                    for( int i = 0; i < qMin( pCodecCtx->channels, 2 ); i++ )
                        if( data->appendData( const_cast<const quint8 *>( decodedFrame->extended_data[i] ), planeSize ) )
                        {
                            isOk = false;
                            break;
                        }
                }

                avpkt.size -= decoderRet;
                avpkt.data += decoderRet;
            }

            data->addTime( av_rescale_q( packet.duration, streamTimeBase, localTimeBase ) );
        }

        av_free_packet( &packet );

        if( data->duration() >= length )
            break;
    }

    av_free_packet( &avpkt );

    avcodec_close( pCodecCtx );
    av_close_input_file( pFormatCtx );
    av_free( decodedFrame );

    return data->duration();
}
#elif LIBAVCODEC_VERSION_MAJOR == 53  && LIBAVCODEC_VERSION_MINOR >= 7 // ffmpeg 0.8
{
    AVFormatContext *pFormatCtx = NULL;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;
    AVPacket packet, avpkt;
    AVRational streamTimeBase = { 1, 1000000 };
    AVRational localTimeBase = { 1, 1000 };

    qint8 *buffer = new qint8[AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    qint32 bufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;

    int outSize = 0;

    int audioStream = 0;
    int decoderRet = 0;

    bool isOk = true;

    if( avformat_open_input( &pFormatCtx, fileName.toLocal8Bit(), NULL, NULL ) < 0 )
    {
        warning() << QLatin1String( "Unable to open input file: " ) + fileName;
        return 0;
    }

    if( av_find_stream_info( pFormatCtx ) < 0 )
    {
        warning() << QLatin1String( "Unable to find stream info: " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    audioStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &pCodec, 0);
    if( audioStream < 0 )
    {
        warning() << QLatin1String( "Unable to find stream: " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    if( !pCodec )
    {
        warning() << QLatin1String( "Unable to find decoder: " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    pCodecCtx = pFormatCtx->streams[audioStream]->codec;

    if( avcodec_open( pCodecCtx, pCodec ) < 0 )
    {
        warning() << QLatin1String( "Unable to open codec " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    streamTimeBase = pFormatCtx->streams[audioStream]->time_base;

    if( data->setupInitial( pCodecCtx->sample_rate,
                            ( pCodecCtx->channels > 1 )? 1 : 0 ) )
        return 0;

    av_init_packet( &avpkt );
    while( !av_read_frame( pFormatCtx, &packet ) && isOk )
    {
        if( packet.stream_index == audioStream )
        {
            avpkt.size = packet.size;
            avpkt.data = packet.data;
            while( avpkt.size > 0 )
            {
                if( bufferSize < qMax( AVCODEC_MAX_AUDIO_FRAME_SIZE, avpkt.size*2 ) )
                {
                    bufferSize = qMax( AVCODEC_MAX_AUDIO_FRAME_SIZE, avpkt.size*2 );
                    delete [] buffer;
                    buffer = new qint8[bufferSize+FF_INPUT_BUFFER_PADDING_SIZE];
                }

                outSize = bufferSize;
                decoderRet = avcodec_decode_audio3( pCodecCtx, ( qint16 * )buffer, &outSize, &avpkt );
                if( decoderRet < 0 )
                {
                    warning() << "Error while decoding.";
                    isOk = false;
                    break;
                }

                if( outSize > 0 )
                    if( data->appendData( ( const quint8 *)buffer, outSize ) )
                    {
                        isOk = false;
                        break;
                    }

                avpkt.size -= decoderRet;
                avpkt.data += decoderRet;
            }

            data->addTime( av_rescale_q( packet.duration, streamTimeBase, localTimeBase ) );
        }

        av_free_packet( &packet );

        if( data->duration() >= length )
            break;
    }

    delete [] buffer;

    av_free_packet( &avpkt );

    avcodec_close( pCodecCtx );
    av_close_input_file( pFormatCtx );

    return data->duration();
}
#elif LIBAVCODEC_VERSION_MAJOR == 52 && LIBAVCODEC_VERSION_MINOR >= 122 // ffmpeg 0.7
{
    AVFormatContext *pFormatCtx = NULL;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;
    AVPacket packet, avpkt;
    AVRational streamTimeBase = { 1, 1000000 };
    AVRational localTimeBase = { 1, 1000 };

    qint8 *buffer = new qint8[AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    qint32 bufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;

    int outSize = 0;

    int audioStream = 0;
    int decoderRet = 0;

    bool isOk = true;

    if( avformat_open_input( &pFormatCtx, fileName.toLocal8Bit(), NULL, NULL ) < 0 )
    {
        warning() << QLatin1String( "Unable to open input file: " ) + fileName;
        return 0;
    }

    if( av_find_stream_info( pFormatCtx ) < 0 )
    {
        warning() << QLatin1String( "Unable to find stream info: " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    audioStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &pCodec, 0);
    if( audioStream < 0 )
    {
        warning() << QLatin1String( "Unable to find stream: " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    if( !pCodec )
    {
        warning() << QLatin1String( "Unable to find decoder: " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    pCodecCtx = pFormatCtx->streams[audioStream]->codec;

    if( avcodec_open( pCodecCtx, pCodec ) < 0 )
    {
        warning() << QLatin1String( "Unable to open codec " ) + fileName;
        av_close_input_file( pFormatCtx );
        return 0;
    }

    streamTimeBase = pFormatCtx->streams[audioStream]->time_base;

    if( data->setupInitial( pCodecCtx->sample_rate,
                            ( pCodecCtx->channels > 1 )? 1 : 0 ) )
        return 0;

    av_init_packet( &avpkt );
    while( !av_read_frame( pFormatCtx, &packet ) && isOk )
    {
        if( packet.stream_index == audioStream )
        {
            avpkt.size = packet.size;
            avpkt.data = packet.data;
            while( avpkt.size > 0 )
            {
                if( bufferSize < qMax( AVCODEC_MAX_AUDIO_FRAME_SIZE, avpkt.size*2 ) )
                {
                    bufferSize = qMax( AVCODEC_MAX_AUDIO_FRAME_SIZE, avpkt.size*2 );
                    delete [] buffer;
                    buffer = new qint8[bufferSize+FF_INPUT_BUFFER_PADDING_SIZE];
                }

                outSize = bufferSize;
                decoderRet = avcodec_decode_audio3( pCodecCtx, ( qint16 * )buffer, &outSize, &avpkt );
                if( decoderRet < 0 )
                {
                    warning() << "Error while decoding.";
                    isOk = false;
                    break;
                }

                if( outSize > 0 )
                    if( data->appendData( ( const quint8 *)buffer, outSize ) )
                    {
                        isOk = false;
                        break;
                    }

                avpkt.size -= decoderRet;
                avpkt.data += decoderRet;
            }

            data->addTime( av_rescale_q( packet.duration, streamTimeBase, localTimeBase ) );
        }

        av_free_packet( &packet );

        if( data->duration() >= length )
            break;
    }

    delete [] buffer;

    av_free_packet( &avpkt );

    avcodec_close( pCodecCtx );
    av_close_input_file( pFormatCtx );

    return data->duration();
}
#else
{
    Q_UNUSED( fileName )
    Q_UNUSED( data )

    warning() << QLatin1String( "Unsupported ffmpeg version. Decoding is abandoned." );

    return 0;
}
#endif


#include "MusicDNSAudioDecoder.moc"