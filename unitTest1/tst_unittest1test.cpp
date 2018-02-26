#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "region_dialog.h"

class UnitTest1Test : public QObject
{
    Q_OBJECT

public:
    UnitTest1Test();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1_data();
    void testCase1();
};

UnitTest1Test::UnitTest1Test()
{
}

void UnitTest1Test::initTestCase()
{
}

void UnitTest1Test::cleanupTestCase()
{
}

void UnitTest1Test::testCase1_data()
{

    QTest::addColumn<int>("number_of_point");
    QTest::addColumn<QString>("restrikt");
        QTest::newRow("1") << 1 << "not ok";
        QTest::newRow("2") << 2 << "not ok";
        QTest::newRow("3") << 3 << "minimum";
        QTest::newRow("4") << 3 << "ok";
   // QTest::addColumn<QString>("expected");
   // QTest::addColumn<QString>("data");
    //QTest::newRow("0") << QString();
}

void UnitTest1Test::testCase1()
{
    //Region_Dialog *Mdialog =new Region_Dialog();
   // Mdialog->exec();
   //  QVector<QPointF> resPoint;
   // resPoint = Mdialog->getResPoint();
   // int num = resPoint.size();
    int num =3;

    QFETCH(int, number_of_point);
    QCOMPARE(num, number_of_point);
    //QVERIFY2(true, "Failure");
}

QTEST_MAIN(UnitTest1Test)

#include "tst_unittest1test.moc"
