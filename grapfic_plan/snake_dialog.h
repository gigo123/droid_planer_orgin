#ifndef SNAKE_DIALOG_H
#define SNAKE_DIALOG_H

#include <QDialog>
#include <QGraphicsItemGroup>
#include "pointt.h"

namespace Ui {
class Snake_Dialog;
}

class Snake_Dialog : public QDialog , public strateCoord
{
    Q_OBJECT

public:
    explicit Snake_Dialog(QPointF startpoint, QWidget *parent=0);
    ~Snake_Dialog();
    QGraphicsScene *painter;
    int wSize;
    qreal Angle;
    //int mDirection;
    int Distance;
    qreal centerX;
    qreal centerY;
     int kordinadX;
     int kordinadY;
     int Aplitude;
     int CrossNumber;
     int RepeatNumber;
    qreal X_height;
  bool setXY;

    QGraphicsItemGroup *group_1;
     QGraphicsItemGroup *group_Point;
       QGraphicsItemGroup *group_Dline;
    int getDistance();
   qreal getAngle();
    bool getSetXY();
    QPointF startPoint;
    QVector<PointOfTrace> getResPath();


    public slots:
    //зміна кількості відрізків
    void changedDistance();
    void changedAngle();
void changedDirection();
     void changedCenterX();
      void changedCenterY();
      void changedAplitude();
      void changedCrossNumber();
      void changedRepeatNumber();
      void coordChange();



private:
    Ui::Snake_Dialog *ui;
    void mainPaiting();
};

#endif // SNAKE_DIALOG_H
