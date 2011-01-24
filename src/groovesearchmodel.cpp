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

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QVariantMap>
#include <QDebug>

#include <qjson/serializer.h> // qjson
#include <qjson/parser.h> // qjson

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

    GrooveRequest request(GrooveClient::instance(), GrooveRequest::more("getSearchResults"));
    request.setMethod("getSearchResults");
    request.setHeader("session", GrooveClientPrivate::instance()->phpCookie().toUtf8());
    request.setHeader("token", GrooveClientPrivate::instance()->grooveMessageToken("getSearchResults"));
    request.setParameter("type", type);
    request.setParameter("query", searchTerm);
    request.post(this, SLOT(searchCompleted()));
}

void GrooveSearchModel::searchCompleted()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (GROOVE_VERIFY(reply, "search returned without a QNetworkReply")) return;

    QJson::Parser parser;
    bool ok;
    QByteArray response = reply->readAll();
    QVariantMap result = parser.parse (response, &ok).toMap();
    if (GROOVE_VERIFY(ok, "error occurred whilst parsing search results")) return;

    QList<GrooveSong *> newSongList;
    foreach (const QVariant &song, result["result"].toList()) {
        QVariantMap songData = song.toMap();

        newSongList.append(new GrooveSong(songData));
    }

    if (!newSongList.count())
        return;

    beginInsertRows(QModelIndex(), 0, newSongList.count() - 1);
    clear();
    m_songs = newSongList;
    endInsertRows();

    qDebug() << Q_FUNC_INFO << "Search found " << m_songs.count() << " songs";
}

GrooveSong *GrooveSearchModel::songByIndex(const QModelIndex &index)
{
    qDebug() << index.row();
    if (GROOVE_VERIFY(index.row() >= 0, "row is negative")) return 0;
    if (GROOVE_VERIFY(index.row() < m_songs.count(), "row is higher than the number of songs model contains")) return 0;

    return m_songs[index.row()];
}
