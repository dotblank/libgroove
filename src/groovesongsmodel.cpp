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
    if (index.column() >= GrooveSongsModelMaxCols || index.row() < 0 || index.row() >= m_songs.count())
        return QVariant();

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
    if (section < 0 || section > GrooveSongsModelMaxCols)
        return QVariant();

    if (orientation == Qt::Vertical)
        return QVariant();

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
    // Decrement reference count on all our songs (this will automatically delete ones with no references left)
    foreach (GrooveSong *song, m_songs)
        song->deref();

    m_songs.clear();
}
