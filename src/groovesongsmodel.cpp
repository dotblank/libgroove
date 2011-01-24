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

// Qt
#include <QUrl>

// Us
#include "groovesongsmodel.h"
#include "groovesong.h"

GrooveSongsModel::GrooveSongsModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    // for QML
    QHash<int, QByteArray> roles;

    for (int i = 0; i < columnCount(QModelIndex()); i++)
        roles[Qt::UserRole + i] = headerData(i, Qt::Horizontal, Qt::DisplayRole).toByteArray();

    qDebug() << Q_FUNC_INFO << "Setting roles to " << roles;
    setRoleNames(roles);
}

static const int GrooveSongsModelMaxCols = 3;

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

    int wantedData = index.column();

    if (role >= Qt::UserRole) {
        // we do this to (mostly transparently) map QML roles to columns
        wantedData = role - Qt::UserRole;
        role = Qt::DisplayRole;
    }

    switch (role) {
    case Qt::DisplayRole:
        switch (wantedData) {
        case 0:
            return m_songs[index.row()]->songName();
        case 1:
            return m_songs[index.row()]->artistName();
        case 2:
            return m_songs[index.row()]->albumName();
        case 3:
            return m_songs[index.row()]->coverArtUrl();
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
            return tr("title");
        case 1:
            return tr("artist");
        case 2:
            return tr("album");
        case 3:
            return tr("coverArtUrl");
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
