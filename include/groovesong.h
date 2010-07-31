/*
 * This file is part of libgroove
 *
 * Copyright (C) 2010 Robin Burchell <viroteck@viroteck.net>
 *
 * This software, including documentation, is protected by copyright.
 * All rights are reserved.
 *
 * Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of the author.
 *
 * This material may also contain confidential information which may not be
 * disclosed to others without the prior written consent of the author.
 */

#ifndef GROOVESONG_H
#define GROOVESONG_H

#include <QSharedData>
#include <QSharedDataPointer>
#include <QString>
#include <QMetaType>
#include <QVariantMap>
class QNetworkReply;

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
    QString coverArtFilename() const;
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

private slots:
    void streamingKeyReady();

signals:
    /*!
        This signal is emitted when this track first has incoming data ready to be processed as a stream.
    */
    void streamingStarted(QNetworkReply *httpStream);

private:
    GrooveSongData *d;
};

#endif // GROOVESONG_H
