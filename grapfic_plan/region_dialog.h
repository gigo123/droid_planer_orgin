#ifndef REGION_DIALOG_H
#define REGION_DIALOG_H

#include <QDialog>
#include <QGraphicsItemGroup>
#include "pointt.h"

namespace Ui {
class Region_Dialog;
}

class Region_Dialog : public QDialog, public strateCoord
{
    Q_OBJECT

public:
    explicit Region_Dialog(QPointF startpoint, QWidget *parent=0);
    ~Region_Dialog();
    QGraphicsScene *painter;
    int wSize;
    qreal Angle;
    //int mDirection;
    int Distance;
    qreal centerX;
    qreal centerY;
     int kordinadX;
     int kordinadY;
   // qreal K_height_line;
    //qreal Line_Height;
    qreal X_height;
    bool setXY;
    QPainterPath ResPath;
    QPainterPath ResRegionPath;

   QVector<QPointF> PointRerionVector; // вектор кординат точкоч
    QGraphicsItemGroup *group_1;
     QGraphicsItemGroup *group_Point;
       QGraphicsItemGroup *group_Dline;

       bool Poinchanged;
    int getDistance();
    bool getSetXY();
    //int getCenterY();
    QVector<PointOfTrace> getResPoint();
    QPointF startPoint;
    QVector<PointOfTrace> getResPath();
void updatePointOfTraceable(); //зміна таблиці відображення точок
//void removePointOfTraceable();


    public slots:
    //зміна кількості відрізків
    void changedDistance();
    void changedAngle();
    void add_point();
     void changedCenterX();
      void changedCenterY();
      void removePointOfTraceable();
      void acceptR();
      void coordChange();


private:
    Ui::Region_Dialog *ui;
    void mainPaiting();
};

#endif // REGION_DIALOG_H
