
#include <QObject>

class tst_GroovePlaylistModel : public QObject
{
Q_OBJECT
private slots:
    void initTestCase();

    void append();
    void insert();
    void removeAt();
    void indexOf();
    void next();
    void previous();
    void currentTrack();

    // bugs
    void testOwnershipRef();

    void cleanupTestCase();
};

