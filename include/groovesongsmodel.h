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

#ifndef GROOVESONGSMODEL_H
#define GROOVESONGSMODEL_H

#include <QAbstractItemModel>

class GrooveSong;

class GrooveSongsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit GrooveSongsModel(QObject *parent = 0);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /*!
        Returns the number of tracks in this playlist.
    */
    int count();

public slots:

    /*!
        Clears this model of all songs, deleting songs which are not referred to in any other models.
    */
    void clear();

protected:
    QList<GrooveSong *> m_songs;
};

#endif // GROOVESONGSMODEL_H
