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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "PlaylistClassicView.h"

#include "debug.h"
#include "PlaylistModel.h"
#include "PlaylistViewCommon.h"
#include "TheInstances.h"
#include "tagdialog.h"
#include "meta/CurrentTrackActionsCapability.h"

#include <KAction>
#include <KMenu>

#include <QHeaderView>
#include <QModelIndex>
#include <QKeyEvent>



Playlist::ClassicView::ClassicView(QWidget * parent)
    : QWidget( parent ), 
    m_treeView( parent )
{
    DEBUG_BLOCK
    QVBoxLayout *layout = new QVBoxLayout(parent);
    setLayout(layout);
    layout->addWidget( &m_treeView );
    m_lineEdit = new KLineEdit( m_editBox );

    m_treeView.setUniformRowHeights(true);
    m_treeView.setSortingEnabled(true);
    m_treeView.header()->setMovable(true);
    m_treeView.header()->setClickable(true);
    m_treeView.setDragEnabled(true);
    m_treeView.setDropIndicatorShown(true);
    m_treeView.setDragDropMode(QAbstractItemView::InternalMove);
    m_treeView.setRootIsDecorated( false );
    m_treeView.setAlternatingRowColors ( true );
    m_treeView.setSelectionMode( QAbstractItemView::ExtendedSelection );
    m_treeView.setModel( The::playlistModel() );



}

Playlist::ClassicView::~ ClassicView()
{
}



void Playlist::ClassicView::setModel( Playlist::Model *model )
{
    DEBUG_BLOCK

    m_model = model;
    //Might need to adjust this for the proxy model
    connect ( &m_treeView, SIGNAL( activated ( const QModelIndex & ) ), m_model , SLOT( playTrack(const QModelIndex & ) ) );   

    m_proxyModel.setSourceModel( model );
    m_treeView.setModel( &m_proxyModel );

    //just for fun
    //header()->hideSection(0);
    m_treeView.header()->hideSection(1);
    m_treeView.header()->hideSection(2);

}

void Playlist::ClassicView::playTrack()
{
    DEBUG_BLOCK

        if(!m_contextIndex)
            return;

    m_model->play( m_contextIndex->row() );
    m_contextIndex = 0;

}

void Playlist::ClassicView::contextMenuEvent( QContextMenuEvent *event )
{
    DEBUG_BLOCK
    QModelIndex index = m_treeView.indexAt(event->pos());
    if( !index.isValid() )
        return;

    m_contextIndex = new QPersistentModelIndex(index);
    //item->setSelected( true );
    event->accept();

    ViewCommon::trackMenu(this, &index, event->globalPos());

}

void Playlist::ClassicView::removeSelection()
{
    DEBUG_BLOCK

    QModelIndexList indexes = m_treeView.selectionModel()->selectedRows() ;
    while ( indexes.size() > 0 ) {

        QModelIndex i = indexes.takeFirst();
        int count = 1;
        while (!indexes.isEmpty() && indexes.takeFirst().row() == i.row() + count)
            ++count;
        m_treeView.model()->removeRows( i.row(), count, i.parent() );
        indexes = m_treeView.selectionModel()->selectedRows() ;
    }
}

void Playlist::ClassicView::editTrackInformation()
{
    DEBUG_BLOCK
        /*
    QModelIndexList selected = m_treeView.selectedIndexes();
    Meta::TrackList tracks;
    foreach(QModelIndex i, selected)
    {
        tracks << m_model->data(i, TrackRole).value< Meta::TrackPtr >();
    }

    TagDialog *dialog = new TagDialog( tracks, this );
    dialog->show();
    */
}

#include "PlaylistClassicView.moc"




