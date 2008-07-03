/******************************************************************************
 * Copyright (C) 2008 Peter ZHOU <peterzhoulei@gmail.com>                     *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License as             *
 * published by the Free Software Foundation; either version 2 of             *
 * the License, or (at your option) any later version.                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.      *
 ******************************************************************************/

#ifndef AMAROK_SCRIPTABLE_SERVICE_MANAGER_SCRIPT_H
#define AMAROK_SCRIPTABLE_SERVICE_MANAGER_SCRIPT_H

#include <QObject>
#include <QtScript>

namespace Amarok
{
    class AmarokScriptableServiceManagerScript : public QObject
    {
        Q_OBJECT

        public:
            AmarokScriptableServiceManagerScript( QScriptEngine* ScriptEngine );
            ~AmarokScriptableServiceManagerScript();
            void slotInit();
            void slotPopulate( int level, int parent_id, QString path );

        public slots:
            bool initService( const QString &name, int levels, const QString &shortDescription, const QString &rootHtml, bool showSearchBar );
            int insertItem( const QString &serviceName, int level, int parentId, const QString &name, const QString &infoHtml, const QString &callbackData, const QString &playableUrl);
            void donePopulating( const QString &serviceName, int parentId );

        signals:
            void init();
            void populate( int level, int parent_id, QString path );
    };
}

#endif
