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

#ifndef GROOVESEARCHREQUEST_H
#define GROOVESEARCHREQUEST_H

#include <QAbstractItemModel>

#include "groovesong.h"

class GrooveSearchModelPrivate;

class GrooveSearchModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit GrooveSearchModel(QObject *parent = 0);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
public slots:
    void searchByArtist(const QString &artist);
    void searchBySong(const QString &song);
    void searchByAlbum(const QString &album);

private:
    /*!
        Performs a search of the given \a type for a \a searchTerm on this model instance.
        Note that searches are asynchronous.
    */
    void searchByHelper(const QString &type, const QString &searchTerm);

    /*!
        List of songs in this model.
    */
    QList<GrooveSong> m_songs;
private slots:
    /*!
        Invoked when a currently running search completes.
    */
    void searchCompleted();
};

#endif // GROOVESEARCHREQUEST_H
