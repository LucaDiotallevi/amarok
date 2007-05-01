/***************************************************************************
 *   Copyright (c) 2007  Nikolaj Hald Nielsen <nhnFreespirit@gmail.com>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02111-1307, USA.          *
 ***************************************************************************/

#include "servicebase.h"

#include "amarok.h"
#include "debug.h"
#include "playlist.h"

#include <khbox.h>

#include <QFrame>
#include <QLabel>

#include <QDirModel>


ServiceBase *ServiceBase::s_instance = 0;

ServiceBase::ServiceBase( const QString &name )
        : KVBox( 0)
{

    DEBUG_BLOCK

    m_name = name;

    m_topPanel = new KVBox( this );

    m_topPanel->setFrameStyle( QFrame::StyledPanel | QFrame::Plain );
    m_topPanel->setLineWidth( 2 );
    m_topPanel->setSpacing( 2 );
    m_topPanel->setMargin( 2 );
    //m_topPanel->setFixedHeight( 50 );

    KHBox * commonPanel = new KHBox ( m_topPanel );


    m_homeButton = new QPushButton( "Home", commonPanel );
    connect( m_homeButton, SIGNAL( clicked( bool ) ), this, SLOT( homeButtonClicked( ) ) );

    QLabel * nameLabel = new QLabel( commonPanel );
    nameLabel->setText( m_name );
    nameLabel->setAlignment(Qt::AlignTop | Qt::AlignCenter);
   
    m_mainSplitter = new QSplitter( Qt::Vertical, this );
    m_contentView = new QTreeView( m_mainSplitter );

    m_contentView->setAlternatingRowColors ( true );
    //m_contentView->setAnimated( true );
    m_contentView->setSortingEnabled( true );
    m_contentView->sortByColumn ( 0, Qt::AscendingOrder );

    m_contentView->setDragEnabled ( true );
    m_contentView->setDragDropMode ( QAbstractItemView::DragOnly );

    
    connect( m_contentView, SIGNAL( pressed ( const QModelIndex & ) ), this, SLOT( treeItemSelected( const QModelIndex & ) ) );
    connect( m_contentView, SIGNAL( doubleClicked ( const QModelIndex & ) ), this, SLOT( itemActivated ( const QModelIndex & ) ) );

    m_infoBox = new KHTMLPart( m_mainSplitter );
    //m_infoBox->view()->widget()->setFrameStyle(QFrame::StyledPanel | QFrame::Plain); //kdelibs error?


    QString infoHtml = "<HTML><HEAD><META HTTP-EQUIV=\"Content-Type\" " 
                       "CONTENT=\"text/html; charset=iso-8859-1\"></HEAD><BODY>";
    infoHtml += "<div align=\"center\"><strong>";
    infoHtml += "Hello there";
    infoHtml += "</strong><br><br>I am ";
    infoHtml += m_name;
    infoHtml += "</div></BODY></HTML>";

    m_infoBox->begin();
    m_infoBox->write(infoHtml);
    m_infoBox->end();

    m_bottomPanel = new KVBox( this );
    m_bottomPanel->setFixedHeight( 50 );
    m_bottomPanel->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    m_bottomPanel->setLineWidth(2);
    m_bottomPanel->setSpacing( 2 );
    m_bottomPanel->setMargin( 2 );


    m_filterModel = new QSortFilterProxyModel( this );
    m_filterModel->setSortCaseSensitivity( Qt::CaseInsensitive );
    m_filterModel->setFilterCaseSensitivity( Qt::CaseInsensitive );




    //QDirModel * dirModel = new QDirModel ( m_contentView );

    //m_contentView->setModel( dirModel );
    //m_contentView->setWindowTitle(QObject::tr("Simple Tree Model"));
    //m_contentView->show();

}


void ServiceBase::showInfo( bool show )
{
    if ( show )
    {
        m_isInfoShown = true;
        m_infoBox->widget()->setMaximumHeight( 2000 );
    }
    else
    {
        m_infoBox->widget()->setMaximumHeight( 0 );
        m_isInfoShown = false;
    }
}


QString ServiceBase::getName( )
{
    return m_name;
}

void ServiceBase::setShortDescription( const QString &shortDescription )
{
    m_shortDescription = shortDescription;
}

QString ServiceBase::getShortDescription( )
{
    return m_shortDescription;
}

void ServiceBase::setLongDescription( const QString &longDescription )
{
    m_longDescription = longDescription;
}

QString ServiceBase::getLongDescription( )
{
    return m_longDescription;
}

void ServiceBase::setIcon( const QIcon &icon )
{
    m_icon = icon;
}

QIcon ServiceBase::getIcon( )
{
    return m_icon;
}

void ServiceBase::homeButtonClicked( ) 
{
    emit( home() );
}

void ServiceBase::itemActivated ( const QModelIndex & index ) {


    debug() << "ServiceBase::itemActivated item double clicked: " << endl;
    if (!index.isValid())
        return;
    else {
      // ServiceModelItemBase * item = static_cast<ServiceModelItemBase*>(m_filterModel->mapToSource( index ).internalPointer() );
      ServiceModelItemBase * item = static_cast<ServiceModelItemBase*>( index.internalPointer() );
      addToPlaylist( item );
    }

    Playlist::instance()->proposePlaylistName( "test" );
    Playlist::instance()->insertMedia( m_urlsToInsert , Playlist::Append);
    m_urlsToInsert.clear();

 }

void ServiceBase::addToPlaylist( ServiceModelItemBase * item ) {
    

    debug() << "ServiceBase::addToPlaylist adding item: " << item->data(0) << endl;
    if (! item->hasChildren() ) {
        QString url = item->getUrl();
        debug() << "ServiceBase::addToPlaylist url to add: " << url << endl;
        if ( !url.isEmpty() ) {
            m_urlsToInsert += KUrl( url );
        }
    } else {

       item->prePopulate();
       item->populate();

        QList<ServiceModelItemBase*> childItems = item->getChildItems();
        for (int i = 0; i < childItems.size(); ++i) {
            addToPlaylist( childItems.at(i) );
        }
    }
}

void ServiceBase::setModel( ServiceModelBase * model ) {

    m_filterModel->setSourceModel( model );
    //m_contentView->setModel( m_filterModel );
    m_contentView->setModel( model );
    m_model  = model;
    connect ( m_model, SIGNAL( infoChanged ( const QString &) ), this, SLOT( infoChanged ( const QString &) ) );
}


ServiceModelBase * ServiceBase::getModel() {
    return m_model;
}

void ServiceBase::treeItemSelected( const QModelIndex & index ) {

    m_model->requestHtmlInfo( index  );
    emit ( selectionChanged ( static_cast<ServiceModelItemBase*>( index.internalPointer() ) ) );

}

void ServiceBase::infoChanged ( const QString &infoHtml ) {


    debug() << "ServiceBase::infoChanged: " << infoHtml << endl;

    m_infoBox->begin( );
    m_infoBox->write( infoHtml ); 
    m_infoBox->end();

}



#include "servicebase.moc"
