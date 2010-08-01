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

#include "groovesongsmodel.h"
#include "groovesong.h"

GrooveSongsModel::GrooveSongsModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

static const int GrooveSongsModelMaxCols = 2;

QModelIndex GrooveSongsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (row < 0 || row >= m_songs.count() || column < 0 || column > GrooveSongsModelMaxCols)
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex GrooveSongsModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}

int GrooveSongsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_songs.count();
}

int GrooveSongsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return GrooveSongsModelMaxCols + 1; // GrooveSongsModelMaxCols is 0 based
}

QVariant GrooveSongsModel::data(const QModelIndex &index, int role) const
{
    if (GROOVE_VERIFY(index.row() >= 0, "row is negative")) return QVariant();
    if (GROOVE_VERIFY(index.row() < m_songs.count(), "row is higher than the number of songs I have")) return QVariant();
    if (GROOVE_VERIFY(index.column() >= 0, "column is negative")) return QVariant();
    if (GROOVE_VERIFY(index.column() <= GrooveSongsModelMaxCols, "column is higher than GrooveSongsModelMaxCols")) return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            return m_songs[index.row()]->songName();
        case 1:
            return m_songs[index.row()]->artistName();
        case 2:
            return m_songs[index.row()]->albumName();
        }
    }

    return QVariant();
}

QVariant GrooveSongsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (GROOVE_VERIFY(section >= 0, "section is negative")) return QVariant();
    if (GROOVE_VERIFY(section <= GrooveSongsModelMaxCols, "section is higher than GrooveSongsModelMaxCols")) return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        switch (section) {
        case 0:
            return tr("Title");
        case 1:
            return tr("Artist");
        case 2:
            return tr("Album");
        }
    }

    return QVariant();
}

/*****/


int GrooveSongsModel::count()
{
    return rowCount(QModelIndex());
}

void GrooveSongsModel::clear()
{
    emit beginResetModel();

    // Decrement reference count on all our songs (this will automatically delete ones with no references left)
    foreach (GrooveSong *song, m_songs)
        song->deref();

    m_songs.clear();

    emit endResetModel();
}
