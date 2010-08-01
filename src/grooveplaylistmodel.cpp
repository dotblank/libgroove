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

#include "grooveplaylistmodel.h"
#include "groovesong.h"

GroovePlaylistModel::GroovePlaylistModel(QObject *parent) :
    GrooveSongsModel(parent),
    m_currentTrack(-1)
{
}

void GroovePlaylistModel::append(GrooveSong *song)
{
    if (GROOVE_VERIFY(song, "song is NULL")) return;

    // ref is made inside insert()
    insert(m_songs.count(), song);
}

void GroovePlaylistModel::insert(int position, GrooveSong *song)
{
    if (GROOVE_VERIFY(song, "song is NULL")) return;
    if (GROOVE_VERIFY(position >= 0, "position is less than 0")) return;
    if (GROOVE_VERIFY(position <= m_songs.count(), "position is greater than it should be")) return;

    // acquire a ref
    song->ref();

    beginInsertRows(QModelIndex(), position, position);
    m_songs.insert(position, song);
    endInsertRows();
}

void GroovePlaylistModel::removeAt(int songPosition)
{
    if (GROOVE_VERIFY(songPosition >= 0, "songPosition is less than 0")) return;
    if (GROOVE_VERIFY(songPosition < m_songs.count(), "songPosition is greater than the number of songs")) return;

    beginRemoveRows(QModelIndex(), songPosition, songPosition);
    m_songs.at(songPosition)->deref();
    m_songs.removeAt(songPosition);
    endRemoveRows();
}

int GroovePlaylistModel::indexOf(GrooveSong *song, int from)
{
    if (GROOVE_VERIFY(song, "song is NULL")) return -1;
    if (GROOVE_VERIFY(from >= 0, "from is negative")) return -1;
    if (GROOVE_VERIFY(from <= m_songs.count(), "from is higher than the playlist length")) return -1;

    for (int i = from; i != m_songs.count(); ++i) {
        if (m_songs.at(i) == song)
            return i;
    }

    return -1;
}

GrooveSong *GroovePlaylistModel::next()
{
    if (++m_currentTrack >= count()) {
        m_currentTrack = count() - 1;
        return 0;
    }

    return m_songs.at(m_currentTrack);
}

GrooveSong *GroovePlaylistModel::previous()
{
    if (--m_currentTrack < 0 || !count()) {
        m_currentTrack = 0;
        return 0;
    }

    return m_songs.at(m_currentTrack);
}

int GroovePlaylistModel::currentTrack()
{
    return m_currentTrack;
}

