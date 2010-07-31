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

#ifndef GROOVECLIENT_P_H
#define GROOVECLIENT_P_H

#include <QObject>
#include <QString>

class GrooveClientPrivate : public QObject
{
Q_OBJECT
public:
    GrooveClientPrivate(QObject *parent) : QObject(parent) { GrooveClientPrivate::m_instance = this; }

    const QString &phpCookie() const;
    const QString &sessionToken() const;

    static GrooveClientPrivate *instance();
signals:
    /*!
        Indicates that the client made a session with Grooveshark successfully.
    */
    void connected();

public slots:
    /*!
        Processes the first stage of the Grooveshark login process.
    */
    void processPHPSessionId();

    /*!
        Process the second stage of the Grooveshark login process.
    */
    void processSessionToken();

    /*!
        Fetch a token to communicate with Grooveshark with.

        You should generally not need to call this in user code, use login() directly.
    */
    void fetchSessionToken();

    QString grooveMessageToken(const QString &method);

private:
    QString m_phpCookie;
    QString m_sessionToken;
    static GrooveClientPrivate *m_instance;
};

#endif // GROOVECLIENT_P_H
