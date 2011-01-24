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

    /*!
        Indicates that the client was unable to connect to Grooveshark.
    */
    void error(Groove::ConnectionError error);

private:
    GrooveClient();
    GrooveClientPrivate *d;
};

#endif // GROOVECLIENT_H
