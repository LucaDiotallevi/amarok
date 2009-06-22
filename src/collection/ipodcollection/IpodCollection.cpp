/*
   Copyright (C) 2008 Alejandro Wainzinger <aikawarazuni@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#define DEBUG_PREFIX "IpodCollection"

#include "IpodCollection.h"
#include "IpodConnectionAssistant.h"
#include "IpodDeviceInfo.h"
#include "MediaDeviceInfo.h"

#include "amarokconfig.h"
#include "Debug.h"

#include <KUrl>

AMAROK_EXPORT_PLUGIN( IpodCollectionFactory )

IpodCollectionFactory::IpodCollectionFactory()
    : MediaDeviceCollectionFactory<IpodCollection> ( new IpodConnectionAssistant() )
{
    DEBUG_BLOCK
    //nothing to do
}

IpodCollectionFactory::~IpodCollectionFactory()
{
    DEBUG_BLOCK
}

//IpodCollection

IpodCollection::IpodCollection(MediaDeviceInfo* info)
: MediaDeviceCollection()
{
    DEBUG_BLOCK
    /** Fetch Info needed to construct IpodCollection */
    debug() << "Getting ipod info";
    IpodDeviceInfo *ipodinfo = qobject_cast<IpodDeviceInfo *>( info );

    debug() << "Getting mountpoint";
    m_mountPoint = ipodinfo->mountpoint();
    debug() << "Getting udi";
    m_udi = ipodinfo->udi();

    debug() << "constructing handler";

    m_handler = new Meta::IpodHandler( this, m_mountPoint );

    emit attemptConnectionDone( m_handler->succeeded() );
}


bool
IpodCollection::possiblyContainsTrack( const KUrl &url ) const
{
    // We could simply check for iPod_Control except that we could actually have multiple ipods connected
    return url.url().startsWith( m_mountPoint ) || url.url().startsWith( "file://" + m_mountPoint );
}

Meta::TrackPtr
IpodCollection::trackForUrl( const KUrl &url )
{
    QString uid = url.url();
    if( uid.startsWith("file://") )
        uid = uid.remove( 0, 7 );
    Meta::TrackPtr ipodTrack = m_trackMap.value( uid );
    return ipodTrack ? ipodTrack : Collection::trackForUrl(url);
}

IpodCollection::~IpodCollection()
{
    DEBUG_BLOCK
}

QString
IpodCollection::collectionId() const
{
     return m_mountPoint;
}

QString
IpodCollection::prettyName() const
{
    return "Ipod at " + m_mountPoint;
}

#include "IpodCollection.moc"

