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

#ifndef GROOVECLIENT_H
#define GROOVECLIENT_H

#include <QNetworkAccessManager>
#include <QObject>

#include "libgroove_global.h"

class GrooveClientPrivate;

class LIBGROOVESHARED_EXPORT GrooveClient : public QObject
{
Q_OBJECT
public:
    static GrooveClient *instance();
    static QNetworkAccessManager *networkManager();

public slots:
    /*!
        Initialises the connection to Grooveshark.
    */
    void establishConnection();

signals:
    /*!
        Indicates that the client made a session with Grooveshark successfully.
    */
    void connected();

private:
    GrooveClient();
    GrooveClientPrivate *d;
};

#endif // GROOVECLIENT_H
