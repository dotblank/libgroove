#include <QtTest>

#include "groovesearchmodel.h"
#include "groovesong.h"

#include "main.h"

void tst_GrooveSearchModel::initTestCase()
{

}

void tst_GrooveSearchModel::cleanupTestCase()
{

}

void tst_GrooveSearchModel::testOffByOnes()
{
    GrooveSearchModel m;
    QCOMPARE(m.songByIndex(m.index(0, 0, QModelIndex())), (GrooveSong *)0);
}

QTEST_MAIN(tst_GrooveSearchModel)
