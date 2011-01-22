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

    QNetworkRequest request;
    request.setUrl(QUrl("http://cowbell.grooveshark.com/more.php?getSearchResults"));
    request.setHeader(request.ContentTypeHeader,QVariant("application/json"));
    QVariantMap jlist;
    QVariantMap header;
    header.insert("client","htmlshark");
    header.insert("clientRevision","20100831");
    header.insert("session", GrooveClientPrivate::instance()->phpCookie().toUtf8());
    header.insert("token", GrooveClientPrivate::instance()->grooveMessageToken("getSearchResults"));
    jlist.insert("method","getSearchResults");
    jlist.insertMulti("header", header);
    QVariantMap param;
    param.insert("type", type);
    param.insert("query", searchTerm);
    jlist.insertMulti("parameters",param);

    QJson::Serializer serializer;
    QNetworkReply *reply = GrooveClient::networkManager()->post(request, serializer.serialize(jlist));
    connect(reply, SIGNAL(finished()), SLOT(searchCompleted()));
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
