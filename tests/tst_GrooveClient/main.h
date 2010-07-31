
#include <QObject>

class tst_GrooveClient : public QObject
{
Q_OBJECT
private slots:
    void initTestCase();

    void establishConnection();

    void cleanupTestCase();
};

