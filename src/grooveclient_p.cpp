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

#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QCryptographicHash>
#include <QDebug>

#include <qjson/serializer.h> // qjson
#include <qjson/parser.h> // qjson

#include "grooveclient.h"
#include "grooveclient_p.h"
#include "grooverequest.h"

GrooveClientPrivate *GrooveClientPrivate::m_instance = NULL;

void GrooveClientPrivate::processPHPSessionId()
{
    qDebug() << Q_FUNC_INFO << "processing";
    QList<QNetworkCookie> cookieList = GrooveClient::networkManager()->cookieJar()->cookiesForUrl(QUrl("http://listen.grooveshark.com"));

    foreach (const QNetworkCookie &cookie, cookieList) {
        if (cookie.name() == "PHPSESSID") {
            m_phpCookie = cookie.value();
            qDebug() << Q_FUNC_INFO << "Got PHP cookie: " << qPrintable(m_phpCookie);
            fetchSessionToken();
            return;
        }
    }

    emit error(Groove::CommunicationError);
    GROOVE_VERIFY(m_phpCookie.length(), "PHP cookie couldn't be set");
}

void GrooveClientPrivate::errorFetchingSessionId(QNetworkReply::NetworkError rpcError)
{
    qDebug() << Q_FUNC_INFO << "Error fetching session token: " << rpcError;
    emit error(Groove::CommunicationError);
}

void GrooveClientPrivate::fetchSessionToken()
{
    GrooveRequest *request = new GrooveRequest(q, GrooveRequest::more("getCommunicationToken"));
    request->setMethod("getCommunicationToken");
    request->setParameter("secretKey", QCryptographicHash::hash(m_phpCookie.toUtf8(), QCryptographicHash::Md5).toHex());
    connect(request, SIGNAL(success(QByteArray)), SLOT(processSessionToken(QByteArray)));
    connect(request, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(errorFetchingSessionToken(QNetworkReply::NetworkError)));
    request->post();
}

void GrooveClientPrivate::processSessionToken(const QByteArray &sessionTokenReply)
{
    bool ok;
    QJson::Parser parser;
    QVariantMap result = parser.parse(sessionTokenReply, &ok).toMap();

    GROOVE_VERIFY(ok, "couldn't parse reply to session token request");
    GROOVE_VERIFY(result["message"].toString().length(), qPrintable(result["message"].toString()));

    m_sessionToken = result["result"].toString();
    qDebug() << Q_FUNC_INFO << "Got session token: " << m_sessionToken;

    if (!ok || !m_sessionToken.length()) {
        qDebug() << Q_FUNC_INFO << "Session token request failed:";
        qDebug() << Q_FUNC_INFO << sessionTokenReply;
        emit error(Groove::SessionError);
        return;
    }

    emit connected();
}

void GrooveClientPrivate::errorFetchingSessionToken(QNetworkReply::NetworkError rpcError)
{
    qDebug() << Q_FUNC_INFO << "Error fetching session token: " << rpcError;
    emit error(Groove::SessionError);
}

const QString &GrooveClientPrivate::sessionToken() const
{
    return m_sessionToken;
}

const QString &GrooveClientPrivate::phpCookie() const
{
    return m_phpCookie;
}

GrooveClientPrivate *GrooveClientPrivate::instance()
{
    return GrooveClientPrivate::m_instance;
}

QString GrooveClientPrivate::grooveMessageToken(const QString &method)
{
    if (GROOVE_VERIFY(m_sessionToken.length(), "made a request to create message without session token")) return QString();

    QString rnum =  QString(qrand() %9 + 48)
                    + QString(qrand() %9 + 48)
                    + QString(qrand() %9 + 48)
                    + QString(qrand() %9 + 48)
                    + QString(qrand() %9 + 48)
                    + QString(qrand() %9 + 48);

    QString messageToken;
    messageToken.append(method);
    messageToken.append(":");
    messageToken.append(m_sessionToken.toAscii());
    messageToken.append(":quitStealinMahShit:");
    messageToken.append(rnum);

    QString rs;
    rs.append(rnum);
    rs.append(QCryptographicHash::hash(messageToken.toAscii(), QCryptographicHash::Sha1).toHex());

    qDebug() << Q_FUNC_INFO << "Produced token " << rs;
    return rs;
}
