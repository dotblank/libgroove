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

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QThreadStorage>
#include <QDebug>

#include "grooveclient.h"
#include "grooveclient_p.h"

GrooveClient::GrooveClient() : d(new GrooveClientPrivate(this))
{
    connect(d, SIGNAL(connected()), this, SIGNAL(connected()));
}

void GrooveClient::establishConnection()
{
    qDebug()  << Q_FUNC_INFO << "Making connection";
    QNetworkRequest loginRequest(QUrl("http://listen.grooveshark.com"));
    QNetworkReply *reply = GrooveClient::networkManager()->get(loginRequest);
    connect(reply, SIGNAL(finished()), d, SLOT(processPHPSessionId()));
}

QThreadStorage<QNetworkAccessManager *> networkManagerPtr;
QNetworkAccessManager *GrooveClient::networkManager()
{
    if (!networkManagerPtr.hasLocalData())
        networkManagerPtr.setLocalData(new QNetworkAccessManager);

    return networkManagerPtr.localData();
}

QThreadStorage<GrooveClient *> grooveClientPtr;
GrooveClient *GrooveClient::instance()
{
    if (!grooveClientPtr.hasLocalData())
        grooveClientPtr.setLocalData(new GrooveClient());

    return grooveClientPtr.localData();
}
