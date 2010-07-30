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
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QCryptographicHash>
#include <QDebug>

#include <serializer.h> // qjson
#include <parser.h> // qjson

#include "grooveclient.h"
#include "grooveclient_p.h"

GrooveClientPrivate *GrooveClientPrivate::m_instance = NULL;

void GrooveClientPrivate::processPHPSessionId()
{
    qDebug("processPHPSessionId");
    QList<QNetworkCookie> cookieList = GrooveClient::networkManager()->cookieJar()->cookiesForUrl(QUrl("http://listen.grooveshark.com"));

    foreach (const QNetworkCookie &cookie, cookieList) {
        if (cookie.name() == "PHPSESSID") {
            m_phpCookie = cookie.value();
            qDebug() << Q_FUNC_INFO << "Got PHP cookie: " << qPrintable(m_phpCookie);
            fetchSessionToken();
            return;
        }
    }

    // FIXME
    qFatal("libgroove: Couldn't get PHP cookie?");
}

void GrooveClientPrivate::fetchSessionToken()
{
    qDebug() << Q_FUNC_INFO << "fetching";
    QNetworkRequest tokenRequest(QUrl("https://cowbell.grooveshark.com/service.php"));
    tokenRequest.setHeader(tokenRequest.ContentTypeHeader, QVariant("application/json"));

    // headers
    QVariantMap vmap;
    vmap.insert("client","gslite");
    vmap.insert("clientRevision","20100412.09");

    // outer map
    QVariantMap jlist;
    jlist.insert("method", "getCommunicationToken");
    jlist.insertMulti("header", vmap);

    // parameters
    vmap.clear();
    vmap.insert("secretKey", QCryptographicHash::hash(m_phpCookie.toUtf8(), QCryptographicHash::Md5).toHex());
    jlist.insertMulti("parameters", vmap);

    // send, hook request
    QJson::Serializer serializer;
    QNetworkReply *reply = GrooveClient::networkManager()->post(tokenRequest, serializer.serialize(jlist));
    connect(reply, SIGNAL(finished()), SLOT(processSessionToken()));
}

void GrooveClientPrivate::processSessionToken()
{
    qDebug() << Q_FUNC_INFO << "got a reply, processing";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT(reply);
    if (!reply) {
        qWarning("GrooveClient::processSessionToken(): got a reply without a reply");
        return;
    }

    bool ok;
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply->readAll(),&ok).toMap();
    if(!ok) {
        // TODO
        qFatal("GrooveClient::processSessionToken(): Error parsing request");
        return;
    }

    if (result["message"].toString().length()) {
        // presume this is an error always
        // TODO
        qFatal("GrooveClient::processSessionToken(): Error from GrooveShark: %s", qPrintable(result["message"].toString()));
        return;
    }

    m_sessionToken = result["result"].toString();
    //qDebug("Got session token: %s", qPrintable(m_sessionToken));
    emit connected();
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
    Q_ASSERT(m_sessionToken.length());

    if (!m_sessionToken.length()) {
        qWarning("requesting grooveMessageToken without an established session, no can do");
        return QString();
    }

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
    return rs;
}
