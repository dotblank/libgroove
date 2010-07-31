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
