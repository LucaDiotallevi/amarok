/*******************************************************************************
* copyright              : (C) 2008 William Viana Soares <vianasw@gmail.com>   *
*                                                                              *
********************************************************************************/

/*******************************************************************************
*                                                                              *
*   This program is free software; you can redistribute it and/or modify       *
*   it under the terms of the GNU General Public License as published by       *
*   the Free Software Foundation; either version 2 of the License, or          *
*   (at your option) any later version.                                        *
*                                                                              *
********************************************************************************/


#ifndef AMAROK_ALBUMSVIEW_H
#define AMAROK_ALBUMSVIEW_H

#include <QtGui/QGraphicsProxyWidget>

class QTreeView;
class QAbstractItemModel;
class QModelIndex;

class AlbumsView : public QGraphicsProxyWidget
{
    Q_OBJECT

    Q_PROPERTY( QAbstractItemModel* model READ model WRITE setModel )
    Q_PROPERTY( QGraphicsWidget* parentWidget READ parentWidget )
    Q_PROPERTY( QString styleSheet READ styleSheet WRITE setStyleSheet )
    Q_PROPERTY( QTreeView* nativeWidget READ nativeWidget )

public:
    explicit AlbumsView( QGraphicsWidget *parent = 0 );
    ~AlbumsView();

    /**
     * Sets a model for this weather view
     *
     * @arg model the model to display
     */
    void setModel( QAbstractItemModel *model );

    /**
     * @return the model shown by this view
     */
    QAbstractItemModel *model();

    /**
     * Sets the stylesheet used to control the visual display of this AlbumsView
     *
     * @arg stylesheet a CSS string
     */
    void setStyleSheet( const QString &stylesheet );

    /**
     * @return the stylesheet currently used with this widget
     */
    QString styleSheet();

    /**
     * @return the native widget wrapped by this AlbumsView
     */
    QTreeView* nativeWidget() const;

protected:
    void resizeEvent( QGraphicsSceneResizeEvent *event );
    
private slots:
    void itemClicked( const QModelIndex &index );
    //void itemDoubleClicked( const QModelIndex &index );

signals:
    //TODO: enqueue a MetaPtr not a QString which isn't unique.
    //void enqueueAlbum( const QString &name );
    //void enqueueTrack( const QString &albumName, const QString &trackName );
    
};

#endif // multiple inclusion guard
