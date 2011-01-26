/*
 * Copyright (C) 2011 Robin Burchell <robin.burchell@collabora.co.uk>
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
#include <QBuffer>
#include <QFile>
#include <QNetworkReply>
#include <QTimer>

// Us
#include "groovestream.h"
#include "groovestream_p.h"

GrooveStream::Private::Private(GrooveStream *pub)
    : QObject(pub)
    , q(pub)
    , cacheFile(0)
    , networkReply(0)
{

}

void GrooveStream::Private::sendFileSignals()
{
    qDebug() << Q_FUNC_INFO << "File backend, emitting done";
    emit q->downloadProgress(cacheFile->size(), cacheFile->size());
    emit q->readyRead();
    emit q->finished();
}

void GrooveStream::Private::processRead()
{
    QIODevice *httpStream = qobject_cast<QIODevice *>(sender());
    if (GROOVE_VERIFY(httpStream, "no stream pointer from sender")) return;

    QByteArray newData = httpStream->readAll();
    networkData.append(newData);

    emit q->downloadProgress(networkData.size(), httpStream->size());
    emit q->readyRead();
}

void GrooveStream::Private::processFinished()
{
    QIODevice *httpStream = qobject_cast<QIODevice *>(sender());
    if (GROOVE_VERIFY(httpStream, "no stream pointer from sender")) return;

    if (networkData.length() == 0) {
        qDebug() << Q_FUNC_INFO << "Ignoring 0 byte cache for " << m_cachePath;
        return;
    }

    QFile cacheFile(m_cachePath);
    cacheFile.open(QIODevice::WriteOnly);
    cacheFile.write(networkData);
    qDebug() << Q_FUNC_INFO << "Cached song to disk: " << m_cachePath << "(" << networkData.size() << " bytes)";

    emit q->finished();
}

void GrooveStream::Private::setCacheFile(QFile *file)
{
    if (GROOVE_VERIFY(file, "pointer is NULL")) return;

    file->setParent(this);
    cacheFile = file;
    QTimer::singleShot(0, this, SLOT(sendFileSignals())); // not instant to give caller a chance to hook signals
    qDebug() << Q_FUNC_INFO << "Starting file read from " << file->fileName();
}

void GrooveStream::Private::setNetworkReply(QNetworkReply *reply)
{
    if (GROOVE_VERIFY(reply, "pointer is NULL")) return;

    reply->setParent(this);
    networkReply = reply;
    networkDataBuffer = new QBuffer(&networkData, this);
    networkDataBuffer->open(QIODevice::ReadOnly);
    connect(reply, SIGNAL(readyRead()), SLOT(processRead()));
    connect(reply, SIGNAL(finished()), SLOT(processFinished()));
    connect(reply, SIGNAL(finished()), q, SIGNAL(finished()));

    qDebug() << Q_FUNC_INFO << "Starting stream from " << reply->url();
}

qint64 GrooveStream::Private::readData(char *data, qint64 maxlen)
{
    if (GROOVE_VERIFY(data, "pointer is NULL")) return 0;

    qint64 bytesRead;
    if (cacheFile) {
        bytesRead = cacheFile->read(data, maxlen);
        qDebug() << Q_FUNC_INFO << "Trying to read " << maxlen << " from file " << cacheFile->fileName() << "; read " << bytesRead;
    } else {
        bytesRead = networkDataBuffer->read(data, maxlen);
        qDebug() << Q_FUNC_INFO << "Trying to read " << maxlen << " from network stream " << networkReply->url() << "; read " << bytesRead;
    }

    return bytesRead;
}

void GrooveStream::Private::setCachePath(const QString &cachePath)
{
    m_cachePath = cachePath;
}
