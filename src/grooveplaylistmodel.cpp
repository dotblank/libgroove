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

#include "grooveplaylistmodel.h"

GroovePlaylistModel::GroovePlaylistModel(QObject *parent) :
    GrooveSongsModel(parent),
    m_currentTrack(0)
{
}

void GroovePlaylistModel::append(GrooveSong *song)
{
    insert(m_songs.count(), song);
}

void GroovePlaylistModel::insert(int position, GrooveSong *song)
{
    beginInsertRows(QModelIndex(), position, position);
    m_songs.insert(position, song);
    endInsertRows();
}

void GroovePlaylistModel::removeAt(int songPosition)
{
    beginRemoveRows(QModelIndex(), songPosition, songPosition);
    m_songs.removeAt(songPosition);
    endRemoveRows();
}

int GroovePlaylistModel::indexOf(GrooveSong *song, int from)
{
    for (int i = from; i != m_songs.count(); ++i) {
        if (m_songs.at(i) == song)
            return i;
    }

    return 0;
}

GrooveSong *GroovePlaylistModel::next()
{
    if (++m_currentTrack >= count())
        return 0;

    return m_songs.at(m_currentTrack);
}

GrooveSong *GroovePlaylistModel::previous()
{
    if (--m_currentTrack < 0 || !count())
        return 0;

    return m_songs.at(m_currentTrack);
}

int GroovePlaylistModel::currentTrack()
{
    return m_currentTrack;
}

