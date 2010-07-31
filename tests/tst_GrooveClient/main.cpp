#include <QtTest>

#include "grooveclient.h"

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

    QVERIFY(spy.count() == 1);
}

QTEST_MAIN(tst_GrooveClient)
