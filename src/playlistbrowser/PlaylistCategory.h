/***************************************************************************
 *   Copyright (c) 2008  Nikolaj Hald Nielsen <nhnFreespirit@gmail.com>    *
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
 
#ifndef PLAYLISTCATEGORY_H
#define PLAYLISTCATEGORY_H

#include "UserPlaylistModel.h"

#include "popupdropper/PopupDropperAction.h"
#include "widgets/Widget.h"

#include <QTreeView>
#include <QWidget>

namespace PlaylistBrowserNS {

/**
The widget that displays playlists in the playlist browser

	@author Nikolaj Hald Nielsen <nhnFreespirit@gmail.com>
*/
class PlaylistCategory : public Amarok::Widget
{
Q_OBJECT
public:
    PlaylistCategory( QWidget * parent );

    ~PlaylistCategory();

private slots:

    void itemActivated ( const QModelIndex & index );
    void showContextMenu ( const QPoint & pos );

private:

    QTreeView * m_playlistView;
    PopupDropperAction * m_deleteAction;
    PopupDropperAction * m_renameAction;

};

}

#endif
