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

    // FIXME
    GROOVE_VERIFY_OR_DIE(m_phpCookie.length(), "PHP cookie couldn't be set");
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
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (GROOVE_VERIFY(reply, "called without a QNetworkReply as sender")) return;

    bool ok;
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply->readAll(),&ok).toMap();

    // TODO
    GROOVE_VERIFY_OR_DIE(ok, "couldn't parse reply to session token request");
    GROOVE_VERIFY_OR_DIE(!result["message"].toString().length(), qPrintable(result["message"].toString()));

    m_sessionToken = result["result"].toString();
    qDebug() << Q_FUNC_INFO << "Got session token: " << m_sessionToken;
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
