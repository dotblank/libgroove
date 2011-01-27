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

#ifndef GROOVESTREAM_P_H
#define GROOVESTREAM_P_H

// Qt
class QBuffer;
class QFile;
class QNetworkReply;

// Us
#include "groovestream.h"

struct GrooveStream::Private : public QObject
{
    Q_OBJECT
public:
    Private(GrooveStream *pub);

    void setCacheFile(QFile *file);
    void setNetworkReply(QNetworkReply *reply);
    void setCachePath(const QString &cachePath);

    qint64 readData(char *data, qint64 maxlen);
    qint64 size() const;

public slots:
    void sendFileSignals();
    void processRead();
    void processFinished();

public:
    GrooveStream *q;
    QFile *cacheFile;
    QNetworkReply *networkReply;
    QByteArray networkData;
    QBuffer *networkDataBuffer;
    QString m_cachePath;
};

#endif // GROOVESTREAM_P_H
