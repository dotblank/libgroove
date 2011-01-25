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

#ifndef GROOVECLIENT_P_H
#define GROOVECLIENT_P_H

// Qt
#include <QObject>
#include <QString>
#include <QNetworkReply>

// Us
#include "libgroove_global.h"
#include "grooveclient.h"

class GrooveClientPrivate : public QObject
{
Q_OBJECT
public:
    GrooveClientPrivate(GrooveClient *parent)
        : QObject(parent)
        , q(parent)
    { GrooveClientPrivate::m_instance = this; }

    const QString &phpCookie() const;
    const QString &sessionToken() const;

    static GrooveClientPrivate *instance();
signals:
    void connected();
    void error(Groove::ConnectionError error);

public slots:
    /*!
        Processes the first stage of the Grooveshark login process.
    */
    void processPHPSessionId();
    void errorFetchingSessionId(QNetworkReply::NetworkError);

    /*!
        Process the second stage of the Grooveshark login process.
    */
    void processSessionToken(const QVariantMap &result);
    void errorFetchingSessionToken(QNetworkReply::NetworkError);

    /*!
        Fetch a token to communicate with Grooveshark with.

        You should generally not need to call this in user code, use login() directly.
    */
    void fetchSessionToken();

    QString grooveMessageToken(const QString &method);

private:
    GrooveClient *q;
    QString m_phpCookie;
    QString m_sessionToken;
    static GrooveClientPrivate *m_instance;
};

#endif // GROOVECLIENT_P_H
