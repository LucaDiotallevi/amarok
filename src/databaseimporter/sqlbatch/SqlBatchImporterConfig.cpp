/****************************************************************************************
 * Copyright (c) 2010 Ralf Engels <ralf-engels@gmx.de>                                  *
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

#include "SqlBatchImporterConfig.h"

#include "core/support/Amarok.h"

#include <KLocale>

#include <QComboBox>
#include <QCompleter>
#include <QDirModel>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

SqlBatchImporterConfig::SqlBatchImporterConfig( QWidget *parent )
    : DatabaseImporterConfig( parent )
{
    QWidget *gridHolder = new QWidget( this );

    QGridLayout *databaseLayout = new QGridLayout( gridHolder );

    QLabel *label = new QLabel( i18n( "Input file" ), gridHolder );
    m_inputFilePathInput = new QLineEdit( gridHolder );
    QCompleter *completer = new QCompleter( this );
    completer->setModel( new QDirModel( completer ) );
    m_inputFilePathInput->setCompleter( completer );
    m_inputFilePathInput->setText( QDir::homePath() + "/result.xml" );
    label->setBuddy( m_inputFilePathInput );

    databaseLayout->addWidget( label, 0, 0 );
    databaseLayout->addWidget( m_inputFilePathInput, 0, 1 );

    gridHolder->setLayout( databaseLayout );

    QWidget *spacer = new QWidget( this );
    spacer->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
}

#include "SqlBatchImporterConfig.moc"
