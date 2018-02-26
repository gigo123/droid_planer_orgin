#ifndef BATERFLY_DIALOG_H
#define BATERFLY_DIALOG_H

#include <QDialog>
#include <QGraphicsItemGroup>
#include "pointt.h"


namespace Ui {
class Baterfly_Dialog;
}

class Baterfly_Dialog : public QDialog, public strateCoord
{
    Q_OBJECT

public:
    explicit Baterfly_Dialog(QPointF startpoint, QWidget *parent = 0);
    ~Baterfly_Dialog();
    QGraphicsScene *painter;
   // int wSize;
    qreal Angle;

    int Distance1;
    int Distance2;
   qreal centerX;
    qreal centerY;
    int kordinadX;
    int kordinadY;
    qreal K_height_line;
    qreal Line_Height;
    qreal X_height;
    bool setXY;


    QGraphicsItemGroup *group_1;
    QGraphicsItemGroup *group_Point;
     QGraphicsItemGroup *group_Dline;
    int getDistance();
    qreal getAngle();
   // int getCenterX();
    bool getSetXY();
    //QVector<int> getResPoint();
        QVector<PointOfTrace> getResPath();

    public slots:
    //зміна кількості відрізків
    void changedDistance1();
    void changedDistance2();
    void changedAngle();
void changedDirection();
    void changedCenterX();
    void changedCenterY();
    void coordChange();


private:
    Ui::Baterfly_Dialog *ui;
    void mainPaiting();
    QPointF startPoint;


};

#endif // BATERFLY_DIALOG_H
