#ifndef ENDLESS_DIALOG_H
#define ENDLESS_DIALOG_H

#include <QDialog>
#include <QGraphicsItemGroup>
#include "pointt.h"

namespace Ui {
class Endless_Dialog;
}

class Endless_Dialog : public QDialog, public strateCoord
{
    Q_OBJECT

public:
    explicit Endless_Dialog( QPointF startpoint, QWidget *parent = 0);
    ~Endless_Dialog();
    QGraphicsScene *painter;
    int wSize;
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
      bool getSetXY();
    //int getCenterY();
      QPointF startPoint;
      QVector<PointOfTrace> getResPath();


    public slots:
    //зміна кількості відрізків
     void changedDirection();
    void changedAngle();
    void changedDistance1();
    void changedDistance2();
     void changedCenterX();
      void changedCenterY();
      void coordChange();

private:
    Ui::Endless_Dialog *ui;
    void mainPaiting();
};

#endif // ENDLESS_DIALOG_H
