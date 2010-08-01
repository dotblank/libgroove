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

#include <QtTest>

#include "grooveclient.h"
#include "grooveclient_p.h"

#include "main.h"

void tst_GrooveClient::initTestCase()
{

}

void tst_GrooveClient::cleanupTestCase()
{

}

void tst_GrooveClient::establishConnection()
{
    QSignalSpy spy(GrooveClient::instance(), SIGNAL(connected()));

    GrooveClient::instance()->establishConnection();

    // check for connection
    int i = 0;
    while (!spy.count() && i != 10) // wait up to 10 seconds for connection, should be plenty
        QTest::qWait(1000);

    // did we connect?
    QVERIFY(spy.count() == 1);

    // check PHP cookie and session token for validity and presence
    QVERIFY(GrooveClientPrivate::instance()->phpCookie().length() == 32);
    QVERIFY(GrooveClientPrivate::instance()->sessionToken().length() == 13);

    foreach (const QChar &schar, GrooveClientPrivate::instance()->phpCookie()) {
        switch (schar.toLatin1()) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':            break;
        default:
            qDebug() << "Bad hex character " << schar;
            QVERIFY(false);
        }
    }

    foreach (const QChar &schar, GrooveClientPrivate::instance()->sessionToken()) {
        switch (schar.toLatin1()) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            break;
        default:
            qDebug() << "Bad hex character " << schar;
            QVERIFY(false);
        }
    }

}

QTEST_MAIN(tst_GrooveClient)
