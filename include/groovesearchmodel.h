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

#include "groovesong.h"
#include "groovesongsmodel.h"

class GrooveSearchModelPrivate;

class GrooveSearchModel : public GrooveSongsModel
{
    Q_OBJECT
public:
    explicit GrooveSearchModel(QObject *parent = 0);
    ~GrooveSearchModel();

    /*!
        Retrieve a song for a given \a index.

        Returns a pointer to the song or 0 if there is an error.
    */
    GrooveSong *songByIndex(const QModelIndex &index);
public slots:
    void searchByArtist(const QString &artist); // XXX: should this really be here? causes problems due to different fields returned?
    void searchBySong(const QString &song);
    void searchByAlbum(const QString &album); // XXX: should this really be here? causes problems due to different fields returned?

private:
    /*!
        Performs a search of the given \a type for a \a searchTerm on this model instance.
        Note that searches are asynchronous.
    */
    void searchByHelper(const QString &type, const QString &searchTerm);

private slots:
    /*!
        Invoked when a currently running search completes.
    */
    void searchCompleted();
};

#endif // GROOVESEARCHREQUEST_H
