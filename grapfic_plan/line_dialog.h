#ifndef LINE_DIALOG_H
#define LINE_DIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include "pointt.h"

namespace Ui {
class Line_Dialog;
}

class Line_Dialog : public QDialog , public strateCoord
{
    Q_OBJECT

public:
    explicit Line_Dialog(QPointF startpoint, QWidget *parent=0);
    ~Line_Dialog();

    QGraphicsScene *painter;
    int wSize;
    qreal Angle;
    //int mDirection;
    int Distance;
    qreal centerX;
    qreal centerY;
     qreal kordinadX;
     qreal kordinadY;
    qreal K_height_line;
    qreal Line_Height;
    qreal X_height;
    QVector<PointOfTrace> Respoint;
    bool setXY;

    QGraphicsItemGroup *group_1;
     QGraphicsItemGroup *group_Point;
       QGraphicsItemGroup *group_Dline;
    int getDistance();
    int getAngle();
    bool getSetXY();
    //int getCenterY();
    QVector<PointOfTrace> getResPoint();

    QPointF startPoint;


public slots:
    //зміна кількості відрізків
    void changedDistance();
    void changedAngle();

     void changedCenterX();
      void changedCenterY();
      void coordChange();



private:
    Ui::Line_Dialog *ui;
    void mainPaiting();
};

#endif // LINE_DIALOG_H
