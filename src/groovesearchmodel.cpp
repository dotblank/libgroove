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

#include <QVariantMap>
#include <QDebug>

#include "groovesearchmodel.h"
#include "groovesong.h"
#include "grooveclient.h"
#include "grooveclient_p.h"
#include "grooverequest.h"

GrooveSearchModel::GrooveSearchModel(QObject *parent) :
    GrooveSongsModel(parent)
{
}

GrooveSearchModel::~GrooveSearchModel()
{
    clear();
}

void GrooveSearchModel::searchByArtist(const QString &artist)
{
    searchByHelper("Artists", artist);
}

void GrooveSearchModel::searchBySong(const QString &song)
{
    searchByHelper("Songs", song);
}

void GrooveSearchModel::searchByAlbum(const QString &album)
{
    searchByHelper("Albums", album);
}

/****/

void GrooveSearchModel::searchByHelper(const QString &type, const QString &searchTerm)
{
    qDebug() << Q_FUNC_INFO << "Searching by " << type << " for " << searchTerm;
    clear();

    emit searchStarted();

    if (searchTerm.trimmed().isEmpty()) {
        qDebug() << Q_FUNC_INFO << "Ignoring empty search";
        emit searchCompleted(0);
        return;
    }

    GrooveRequest *request = new GrooveRequest(GrooveClient::instance(), GrooveRequest::more("getSearchResults"));
    request->setMethod("getSearchResults");
    request->setHeader("session", GrooveClientPrivate::instance()->phpCookie().toUtf8());
    request->setHeader("token", GrooveClientPrivate::instance()->grooveMessageToken("getSearchResults"));
    request->setParameter("type", type);
    request->setParameter("query", searchTerm);
    connect(request, SIGNAL(success(QVariantMap)), SLOT(processSearchCompleted(QVariantMap)));
    connect(request, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(searchError(QNetworkReply::NetworkError)));
    request->post();
}

void GrooveSearchModel::processSearchCompleted(const QVariantMap &result)
{
    if (result.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Error occured whilst parsing search results";
        return;
    }

    QList<GrooveSong *> newSongList;
    foreach (const QVariant &song, result["result"].toMap()["Songs"].toList()) {
        QVariantMap songData = song.toMap();

        newSongList.append(new GrooveSong(songData));
    }

    qDebug() << Q_FUNC_INFO << "Search found " << newSongList.count() << " songs";
    emit searchCompleted(newSongList.count());

    if (!newSongList.count())
        return;

    beginInsertRows(QModelIndex(), 0, newSongList.count() - 1);
    clear();
    m_songs = newSongList;
    endInsertRows();
}

void GrooveSearchModel::searchError(QNetworkReply::NetworkError rpcError)
{
    qDebug() << Q_FUNC_INFO << "Search failed: " << rpcError;
}

GrooveSong *GrooveSearchModel::songByIndex(const QModelIndex &index)
{
    qDebug() << index.row();
    if (GROOVE_VERIFY(index.row() >= 0, "row is negative")) return 0;
    if (GROOVE_VERIFY(index.row() < m_songs.count(), "row is higher than the number of songs model contains")) return 0;

    return m_songs[index.row()];
}
