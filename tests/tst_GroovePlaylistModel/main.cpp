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

#include "grooveplaylistmodel.h"
#include "groovesong.h"

#include "main.h"

void tst_GroovePlaylistModel::initTestCase()
{

}

void tst_GroovePlaylistModel::cleanupTestCase()
{

}

void tst_GroovePlaylistModel::append()
{
    GroovePlaylistModel gpm;

    // initial
    QVERIFY(gpm.count() == 0);

    // append(1), verify
    GrooveSong *g1 = new GrooveSong(QVariantMap());
    gpm.append(g1);
    QVERIFY(gpm.count() == 1);
    QCOMPARE(gpm.indexOf(g1), 0);

    // append(2), verify
    GrooveSong *g2 = new GrooveSong(QVariantMap());
    gpm.append(g2);
    QVERIFY(gpm.count() == 2);
    QCOMPARE(gpm.indexOf(g2), 1);
}

void tst_GroovePlaylistModel::insert()
{
    GroovePlaylistModel gpm;

    // initial
    QVERIFY(gpm.count() == 0);

    GrooveSong *g1 = new GrooveSong(QVariantMap());
    gpm.insert(0, g1);
    QVERIFY(gpm.count() == 1);
    QCOMPARE(gpm.indexOf(g1), 0);

    GrooveSong *g2 = new GrooveSong(QVariantMap());
    gpm.insert(0, g2);
    QVERIFY(gpm.count() == 2);
    QCOMPARE(gpm.indexOf(g2), 0);

    GrooveSong *g3 = new GrooveSong(QVariantMap());
    gpm.insert(2, g3);
    QVERIFY(gpm.count() == 3);
    QCOMPARE(gpm.indexOf(g3), 2);
}

void tst_GroovePlaylistModel::removeAt()
{
    GroovePlaylistModel gpm;

    // initial
    QVERIFY(gpm.count() == 0);

    // add
    GrooveSong *g1 = new GrooveSong(QVariantMap());
    gpm.insert(0, g1);
    QVERIFY(gpm.count() == 1);
    QCOMPARE(gpm.indexOf(g1), 0);

    // remove
    gpm.removeAt(0);
    QVERIFY(gpm.count() == 0);
    QCOMPARE(gpm.indexOf(g1), -1);

    // insert
    GrooveSong *g2 = new GrooveSong(QVariantMap());
    gpm.insert(0, g2);
    QVERIFY(gpm.count() == 1);
    QCOMPARE(gpm.indexOf(g2), 0);

    GrooveSong *g3 = new GrooveSong(QVariantMap());
    gpm.insert(1, g3);
    QVERIFY(gpm.count() == 2);
    QCOMPARE(gpm.indexOf(g3), 1);

    // remove
    gpm.removeAt(1);
    QVERIFY(gpm.count() == 1);
    QCOMPARE(gpm.indexOf(g3), -1);
    QCOMPARE(gpm.indexOf(g2), 0);
}

void tst_GroovePlaylistModel::indexOf()
{
    // Also covered by append()/insert() tests.
    GroovePlaylistModel gpm;

    // initial sanity checks
    QVERIFY(gpm.count() == 0);
    QCOMPARE(gpm.indexOf((GrooveSong*)1 /* can't pass NULL, that will assert */), -1);
}

void tst_GroovePlaylistModel::next()
{
    GroovePlaylistModel gpm;

    // append(1), verify
    GrooveSong *g1 = new GrooveSong(QVariantMap());
    gpm.append(g1);
    QCOMPARE(gpm.next(), g1);

    // append(2), verify
    GrooveSong *g2 = new GrooveSong(QVariantMap());
    gpm.append(g2);
    QCOMPARE(gpm.next(), g2);

    // make sure we don't wrap
    QVERIFY(gpm.next() == 0);
}

void tst_GroovePlaylistModel::previous()
{
    GroovePlaylistModel gpm;

    // sanity check
    QVERIFY(gpm.currentTrack() == -1);

    // append(1), verify
    GrooveSong *g1 = new GrooveSong(QVariantMap());
    gpm.append(g1);
    QCOMPARE(gpm.next(), g1);
    QCOMPARE(gpm.currentTrack(), 0);

    // append(2), verify
    GrooveSong *g2 = new GrooveSong(QVariantMap());
    gpm.append(g2);
    QCOMPARE(gpm.next(), g2);
    QCOMPARE(gpm.currentTrack(), 1);

    // we should be at the end of the list now, go back to g1
    QCOMPARE(gpm.previous(), g1);
    QCOMPARE(gpm.currentTrack(), 0);

    // and we shouldn't be able to go back past the start
    QVERIFY(gpm.previous() == 0);
    QCOMPARE(gpm.currentTrack(), 0);
}

void tst_GroovePlaylistModel::currentTrack()
{
    // covered by ::previous()
}


// Test that GroovePlaylistModel correctly acquires a reference to GrooveSong.
class GroovePlaylistBugTestSong : public GrooveSong
{
public:
    GroovePlaylistBugTestSong(const QVariantMap &data) : GrooveSong(data) {}
    friend class tst_GroovePlaylistModel;
};

void tst_GroovePlaylistModel::testOwnershipRef()
{
    GroovePlaylistModel gpm;

    GroovePlaylistBugTestSong *g = new GroovePlaylistBugTestSong(QVariantMap());
    QVERIFY(g->d->m_refCount == 1); // there is always a magic initial reference

    gpm.append(g);
    QVERIFY(g->d->m_refCount == 2); // make sure there's one more after we take ownership
}


QTEST_MAIN(tst_GroovePlaylistModel)
