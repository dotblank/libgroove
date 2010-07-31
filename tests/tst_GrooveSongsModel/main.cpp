#include <QtTest>

#include "groovesongsmodel.h"
#include "groovesong.h"

#include "main.h"

void tst_GrooveSongsModel::initTestCase()
{

}

void tst_GrooveSongsModel::cleanupTestCase()
{

}

void tst_GrooveSongsModel::testOffByOnes()
{
    GrooveSongsModel m;
    QCOMPARE(m.index(0, 0, QModelIndex()), QModelIndex());
    QCOMPARE(m.data(m.index(0, 0, QModelIndex()), Qt::DisplayRole), QVariant());
}

QTEST_MAIN(tst_GrooveSongsModel)
