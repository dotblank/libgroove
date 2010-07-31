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

#ifndef GROOVEPLAYLISTMODEL_H
#define GROOVEPLAYLISTMODEL_H

#include "groovesongsmodel.h"
class GrooveSong;

class GroovePlaylistModel : public GrooveSongsModel
{
    Q_OBJECT
public:
    explicit GroovePlaylistModel(QObject *parent = 0);

public slots:
    /*!
        Appends a given \a song to the end of this playlist.

        \sa insert()
    */
    void append(GrooveSong *song);

    /*!
        Insert a given \a song at a specified \a position into this playlist.
    */
    void insert(int position, GrooveSong *song);

    /*!
        Removes track at a given \a songPosition from this playlist.
    */
    void removeAt(int songPosition);

    /*!
        Finds the position of a given \a song in this playlist, searching forward from the index \a from.
    */
    int indexOf(GrooveSong *song, int from = 0);

    /*!
        Returns the next track to be played from this playlist, or 0 if there is no next track.
    */
    GrooveSong *next();

    /*!
        Returns the track played previously from this playlist, or 0 if there is no previous track.
    */
    GrooveSong *previous();

    /*!
        Returns the index of the currently playing track.
    */
    int currentTrack();

private:
    int m_currentTrack;
};

#endif // GROOVEPLAYLISTMODEL_H
