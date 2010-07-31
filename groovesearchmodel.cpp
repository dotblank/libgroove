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

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QVariantMap>
#include <QDebug>

#include <serializer.h> // qjson
#include <parser.h> // qjson

#include "groovesearchmodel.h"
#include "groovesong.h"
#include "grooveclient.h"
#include "grooveclient_p.h"

GrooveSearchModel::GrooveSearchModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

GrooveSearchModel::~GrooveSearchModel()
{
    qDeleteAll(m_songs);
    m_songs.clear();
}

void GrooveSearchModel::searchByArtist(const QString &artist)
{
    emit beginResetModel();
    qDeleteAll(m_songs);
    m_songs.clear();
    emit endResetModel();

    searchByHelper("Artists", artist);
}

void GrooveSearchModel::searchBySong(const QString &song)
{
    emit beginResetModel();
    qDeleteAll(m_songs);
    m_songs.clear();
    emit endResetModel();

    searchByHelper("Songs", song);
}

void GrooveSearchModel::searchByAlbum(const QString &album)
{
    emit beginResetModel();
    qDeleteAll(m_songs);
    m_songs.clear();
    emit endResetModel();

    searchByHelper("Albums", album);
}

#define GROOVE_SEARCHMODELMAXCOLS 2

QModelIndex GrooveSearchModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || row > m_songs.count() || column < 0 || column > GROOVE_SEARCHMODELMAXCOLS)
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex GrooveSearchModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int GrooveSearchModel::rowCount(const QModelIndex &parent) const
{
    return m_songs.count();
}

int GrooveSearchModel::columnCount(const QModelIndex &parent) const
{
    return GROOVE_SEARCHMODELMAXCOLS + 1; // GROOVE_SEARCHMODELMAXCOLS is 0 based
}

QVariant GrooveSearchModel::data(const QModelIndex &index, int role) const
{
    if (index.column() > GROOVE_SEARCHMODELMAXCOLS || index.row() < 0 || index.row() > m_songs.count())
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

QVariant GrooveSearchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 0 || section > GROOVE_SEARCHMODELMAXCOLS)
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


/****/

void GrooveSearchModel::searchByHelper(const QString &type, const QString &searchTerm)
{
    qDebug() << Q_FUNC_INFO << "Searching by " << type << " for " << searchTerm;

    QNetworkRequest request;
    request.setUrl(QUrl("http://cowbell.grooveshark.com/more.php?getSearchResults"));
    request.setHeader(request.ContentTypeHeader,QVariant("application/json"));
    QVariantMap jlist;
    QVariantMap header;
    header.insert("client","gslite");
    header.insert("clientRevision","20100412.09");
    header.insert("session", GrooveClientPrivate::instance()->phpCookie().toUtf8());
    header.insert("token", GrooveClientPrivate::instance()->grooveMessageToken("getSearchResults"));
    jlist.insert("method","getSearchResults");
    jlist.insertMulti("header", header);
    QVariantMap param;
    param.insert("type", type);
    param.insert("query", searchTerm);
    jlist.insertMulti("parameters",param);

    QJson::Serializer serializer;
    QNetworkReply *reply = GrooveClient::networkManager()->post(request, serializer.serialize(jlist));
    connect(reply, SIGNAL(finished()), SLOT(searchCompleted()));
}

void GrooveSearchModel::searchCompleted()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) {
        qWarning("GrooveSearchModel::searchCompleted(): got searchCompleted() with no reply?");
        return;
    }
    Q_ASSERT(reply);

    QJson::Parser parser;
    bool ok;
    QByteArray response = reply->readAll();
    QVariantMap result = parser.parse (response, &ok).toMap();
    if (!ok) {
        // TODO
      qWarning("An error occurred during parsing");
      return;
    }

    QList<GrooveSong *> newSongList;
    foreach (const QVariant &song, result["result"].toList()) {
        QVariantMap songData = song.toMap();

        newSongList.append(new GrooveSong(songData));
    }

    if (!newSongList.count())
        return;

    beginInsertRows(QModelIndex(), 0, newSongList.count() - 1);
    qDeleteAll(m_songs);
    m_songs = newSongList;
    endInsertRows();

    qDebug() << Q_FUNC_INFO << "Search found " << m_songs.count() << " songs";
}

GrooveSong *GrooveSearchModel::songByIndex(const QModelIndex &index)
{
    if (index.row() < 0 || index.row() > m_songs.count()) {
        qDebug() << Q_FUNC_INFO << "Requested a bad row index: " << index.row();
        return NULL;
    }

    return m_songs[index.row()];
}
