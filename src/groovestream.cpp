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

// Us
#include "groovestream.h"
#include "groovestream_p.h"

GrooveStream::GrooveStream(QFile *cacheFile)
    : d(new Private(this))
{
    d->setCacheFile(cacheFile);
    open(QIODevice::ReadOnly);
}

GrooveStream::GrooveStream(QNetworkReply *reply, const QString &cachePath)
    : d(new Private(this))
{
    d->setCachePath(cachePath);
    d->setNetworkReply(reply);
    open(QIODevice::ReadOnly);
}

qint64 GrooveStream::readData(char *data, qint64 maxlen)
{
    return d->readData(data, maxlen);
}

qint64 GrooveStream::writeData(const char *data, qint64 len)
{
    // nope, we're not writable
    Q_UNUSED(data)
    Q_UNUSED(len)
    return -1;
}
