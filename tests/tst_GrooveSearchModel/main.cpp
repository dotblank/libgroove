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
    QCOMPARE(m.index(0, 0, QModelIndex()), QModelIndex());
    QCOMPARE(m.songByIndex(m.index(0, 0, QModelIndex())), (GrooveSong *)0);
    QCOMPARE(m.data(m.index(0, 0, QModelIndex()), Qt::DisplayRole), QVariant());
}

QTEST_MAIN(tst_GrooveSearchModel)
