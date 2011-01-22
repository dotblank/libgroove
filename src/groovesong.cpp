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

#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <qjson/serializer.h>
#include <qjson/parser.h>

#include "groovesong.h"
#include "grooveclient.h"
#include "grooveclient_p.h"

GrooveSong::GrooveSong(const QVariantMap &data)
{
    d = new GrooveSongData;
    d->m_data = data;
    d->m_refCount = QAtomicInt(0);

    // initial refcount on creation
    ref();
}

GrooveSong::~GrooveSong()
{
    delete d;
}

void GrooveSong::ref()
{
    d->m_refCount.ref();
}

void GrooveSong::deref()
{
    if (!d->m_refCount.deref())
        deleteLater();
}

QString GrooveSong::songID() const
{
    return d->m_data["SongID"].toString();
}

int GrooveSong::albumID() const
{
    return d->m_data["AlbumID"].toInt();
}

int GrooveSong::artistID() const
{
    return d->m_data["ArtistID"].toInt();
}

int GrooveSong::genreID() const
{
    return d->m_data["GenreID"].toInt();
}

QString GrooveSong::name() const
{
    return d->m_data["Name"].toString();
}

QString GrooveSong::songName() const
{
    return d->m_data["SongName"].toString();
}

QString GrooveSong::albumName() const
{
    return d->m_data["AlbumName"].toString();
}

QString GrooveSong::artistName() const
{
    return d->m_data["ArtistName"].toString();
}

int GrooveSong::year() const
{
    return d->m_data["Year"].toInt();
}

int GrooveSong::trackNum() const
{
    return d->m_data["TrackNum"].toInt();
}

QString GrooveSong::coverArtFilename() const
{
    return d->m_data["CoverArtFilename"].toString();
}

int GrooveSong::TSAdded() const
{
    return d->m_data["TSAdded"].toInt();
}

int GrooveSong::avgRating() const
{
    return d->m_data["AvgRating"].toInt();
}

int GrooveSong::avgDuration() const
{
    return d->m_data["AvgDuration"].toInt();
}

int GrooveSong::estimateDuration() const
{
    return d->m_data["EstimateDuration"].toInt();
}

int GrooveSong::flags() const
{
    return d->m_data["Flags"].toInt();
}

int GrooveSong::isLowBitrateAvailable() const
{
    return d->m_data["IsLowBitrateAvailable"].toInt();
}

int GrooveSong::songVerified() const
{
    return d->m_data["SongVerified"].toInt();
}

int GrooveSong::albumVerified() const
{
    return d->m_data["AlbumVerified"].toInt();
}

int GrooveSong::artistVerified() const
{
    return d->m_data["ArtistVerified"].toInt();
}

int GrooveSong::popularity() const
{
    return d->m_data["Popularity"].toInt();
}

int GrooveSong::albumPopularity() const
{
    return d->m_data["AlbumPopularity"].toInt();
}

int GrooveSong::artistPopularity() const
{
    return d->m_data["ArtistPopularity"].toInt();
}

int GrooveSong::songPlays() const
{
    return d->m_data["SongPlays"].toInt();
}

int GrooveSong::artistPlays() const
{
    return d->m_data["ArtistPlays"].toInt();
}

int GrooveSong::songClicks() const
{
    return d->m_data["SongClicks"].toInt();
}

int GrooveSong::albumClicks() const
{
    return d->m_data["AlbumClicks"].toInt();
}

int GrooveSong::artistClicks() const
{
    return d->m_data["ArtistClicks"].toInt();
}

int GrooveSong::querySongClicks() const
{
    return d->m_data["QuerySongClicks"].toInt();
}

int GrooveSong::queryAlbumClicks() const
{
    return d->m_data["QueryAlbumClicks"].toInt();
}

int GrooveSong::queryArtistClicks() const
{
    return d->m_data["QueryArtistClicks"].toInt();
}

int GrooveSong::sphinxWeight() const
{
    return d->m_data["sphinxWeight"].toInt();
}

int GrooveSong::score() const
{
    return d->m_data["Score"].toInt();
}

QString GrooveSong::DSName() const
{
    return d->m_data["DSName"].toString();
}

QString GrooveSong::DALName() const
{
    return d->m_data["DALName"].toString();
}

QString GrooveSong::DAName() const
{
    return d->m_data["DAName"].toString();
}

int GrooveSong::TSAddedInt() const
{
    return d->m_data["TSAddedInt"].toInt();
}

int GrooveSong::rank() const
{
    return d->m_data["Rank"].toInt();
}

void GrooveSong::startStreaming()
{
    qDebug() << Q_FUNC_INFO << "Started streaming for " << songName() << "(id: " << songID() << ")";

    QNetworkRequest request;
    request.setUrl(QUrl("http://listen.grooveshark.com/more.php?getStreamKeyFromSongIDEx"));
    request.setHeader(request.ContentTypeHeader,QVariant("application/json"));
    QVariantMap jlist;
    QVariantMap header;
    QVariantMap country;
    country.insert("CC1","0");
    country.insert("CC3","0");
    country.insert("ID","223");
    country.insert("CC2","0");
    country.insert("CC4","1073741824");
    //header.insert("uuid","DEA8E133-2080-F666-4B38-9465187B20A9");
    header.insert("session", GrooveClientPrivate::instance()->phpCookie().toUtf8());
    header.insert("client","jsqueue");
    header.insert("clientRevision","20101012.37");
    header.insert("token", GrooveClientPrivate::instance()->grooveMessageToken("getStreamKeyFromSongIDEx"));
    header.insertMulti("country",country);
    jlist.insert("method","getStreamKeyFromSongIDEx");
    jlist.insertMulti("header",header);
    QVariantMap param;
    param.insertMulti("country",country);
    param.insert("mobile",false);
    param.insert("songID", songID().toAscii());
    param.insert("prefetch",false);
    jlist.insertMulti("parameters",param);

    QJson::Serializer serializer;
    QNetworkReply *reply = GrooveClient::instance()->networkManager()->post(request, serializer.serialize(jlist));
    connect(reply, SIGNAL(finished()), SLOT(streamingKeyReady()));
    // TODO: error handling
}

void GrooveSong::streamingKeyReady()
{
    qDebug() << Q_FUNC_INFO << "Ready for " << songName();
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (GROOVE_VERIFY(reply, "reply with streaming key, without a QNetworkReply")) return;

    QJson::Parser parser;
    bool ok;
    QByteArray response = reply->readAll();
    QVariantMap result = parser.parse(response, &ok).toMap();
    qDebug() << Q_FUNC_INFO << response;
    if (GROOVE_VERIFY(ok, "error occured whilst parsing streaming key reply")) return;
    QVariantMap results = result["result"].toMap();

    QNetworkRequest req;
    req.setUrl(QUrl(QString("http://") + results["ip"].toString() + "/stream.php"));
    req.setHeader(req.ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

    qDebug() << Q_FUNC_INFO << "Sending request to " << req.url().toString() << " to start stream";

    QNetworkReply *streamingReply = GrooveClient::instance()->networkManager()->post(req,
                                                                                     QString("streamKey=" +
                                                                                             results["streamKey"].toString()).toAscii());
    emit streamingStarted(streamingReply);
}
