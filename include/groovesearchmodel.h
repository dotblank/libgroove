/*
 * Copyright (C) 2010 Robin Burchell <robin.burchell@collabora.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef GROOVESEARCHREQUEST_H
#define GROOVESEARCHREQUEST_H

// Qt
#include <QNetworkReply>

// Us
#include "groovesong.h"
#include "groovesongsmodel.h"

class GrooveSearchModelPrivate;

class GrooveSearchModel : public GrooveSongsModel
{
    Q_OBJECT
public:
    explicit GrooveSearchModel(QObject *parent = 0);
    ~GrooveSearchModel();

    /*!
        Retrieve a song for a given \a index.

        Returns a pointer to the song or 0 if there is an error.
    */
    GrooveSong *songByIndex(const QModelIndex &index);
public slots:
    void searchByArtist(const QString &artist); // XXX: should this really be here? causes problems due to different fields returned?
    void searchBySong(const QString &song);
    void searchByAlbum(const QString &album); // XXX: should this really be here? causes problems due to different fields returned?

signals:
    void searchCompleted(int numOfResults);
    void searchStarted();

private:
    /*!
        Performs a search of the given \a type for a \a searchTerm on this model instance.
        Note that searches are asynchronous.
    */
    void searchByHelper(const QString &type, const QString &searchTerm);

private slots:
    /*!
        Invoked when a currently running search completes.
    */
    void processSearchCompleted(const QVariantMap &result);

    void searchError(QNetworkReply::NetworkError rpcError);
};

#endif // GROOVESEARCHREQUEST_H
