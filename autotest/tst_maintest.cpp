#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class MainTest : public QObject
{
    Q_OBJECT

public:
    MainTest();
    ~MainTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

MainTest::MainTest()
{

}

MainTest::~MainTest()
{

}

void MainTest::initTestCase()
{

}

void MainTest::cleanupTestCase()
{

}

void MainTest::test_case1()
{

}

QTEST_MAIN(MainTest)

#include "tst_maintest.moc"
