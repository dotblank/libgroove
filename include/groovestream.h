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

#ifndef GROOVESTREAM_H
#define GROOVESTREAM_H

// Qt
#include <QFile>
#include <QNetworkReply>
#include <QObject>

// Us
#include "groovesong.h"

class GrooveStream : public QIODevice
{
    Q_OBJECT
    GrooveStream(QFile *cacheFile);
    GrooveStream(QNetworkReply *reply, const QString &cachePath);

public:
    qint64 size() const;

protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

signals:
    void finished();
    void error(QNetworkReply::NetworkError);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    class Private;
    Private *d;
    friend class GrooveSong;
};

#endif // GROOVESTREAM_H
