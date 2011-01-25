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

#ifndef GROOVESONG_H
#define GROOVESONG_H

#include <QSharedData>
#include <QSharedDataPointer>
#include <QString>
#include <QMetaType>
#include <QVariantMap>
#include <QUrl>
#include <QNetworkReply>

#include "libgroove_global.h"

class LIBGROOVESHARED_EXPORT GrooveSongData
{
public:
    QVariantMap m_data;
    QAtomicInt m_refCount;
};

class LIBGROOVESHARED_EXPORT GrooveSong : public QObject
{
Q_OBJECT
public:
    GrooveSong(const QVariantMap &data);
    ~GrooveSong();

    void ref();
    void deref();

    QString songID() const;
    int albumID() const;
    int artistID() const;
    int genreID() const;
    QString name() const;
    QString songName() const;
    QString albumName() const;
    QString artistName() const;
    void setYear(int year);
    int year() const;
    int trackNum() const;
    QUrl coverArtUrl() const;
    int TSAdded() const;
    int avgRating() const;
    int avgDuration() const;
    int estimateDuration() const;
    int flags() const;
    int isLowBitrateAvailable() const;
    int songVerified() const;
    int albumVerified() const;
    int artistVerified() const;
    int popularity() const;
    int albumPopularity() const;
    int artistPopularity() const;
    int songPlays() const;
    int artistPlays() const;
    int songClicks() const;
    int albumClicks() const;
    int artistClicks() const;
    int querySongClicks() const;
    int queryAlbumClicks() const;
    int queryArtistClicks() const;
    int sphinxWeight() const;
    int score() const;
    QString DSName() const;
    QString DALName() const;
    QString DAName() const;
    int TSAddedInt() const;
    int rank() const;

public slots:
    /*!
        Calling this will initiate communication with Grooveshark to stream this track.

        Streaming is not an instantaneous process, it may take a while before streaming is ready.

        You will recieve notification through the streamingStarted() signal.

        \sa streamingStarted()
    */
    void startStreaming();

signals:
    /*!
        This signal is emitted when this track first has incoming data ready to be processed as a stream.
    */
    void streamingStarted(QIODevice *httpStream);

    void streamingError(QNetworkReply::NetworkError);

private slots:
    // TODO: these should be moved *OUT* of here.
    void streamingKeyReady(const QVariantMap &result);
    void streamingKeyError(QNetworkReply::NetworkError rpcError);

// XXX: protected for tst_GroovePlaylistModel::testOwnershipRef(), would be nice if we could make this private again.
protected:
    GrooveSongData *d;
};

#endif // GROOVESONG_H
