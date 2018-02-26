#ifndef CIRCLEDIALOG_H
#define CIRCLEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QPainterPath>
#include "pointt.h"

namespace Ui {
class CircleDialog;
}

class CircleDialog : public QDialog , public strateCoord
{
    Q_OBJECT

public:
    explicit CircleDialog(QPointF startpoint, QWidget *parent=0);
    ~CircleDialog();
    QGraphicsScene *painter;
    int wSize;
    int aproximNu;
    int mDirection;
    int Radius;
    int centerXR;
    int centerYR;
    qreal centerX;
    qreal centerY;
    bool setXY;
    QPainterPath ResPath;


    QGraphicsItemGroup *group_1;

    int getDirection();
   bool getSetXY();

   QPointF startPoint;
   QVector<PointOfTrace> getResPath();



private:
    Ui::CircleDialog *ui;
    void mainPaiting();
    QGraphicsItemGroup *pointEnum;

public slots:
    //зміна кількості відрізків
    void changedAproximN();
    void changedDirectionO();
    void changedDirectionR();
     void changedRadius();
     void changedCenterX();
      void changedCenterY();
      void coordChange();


};

#endif // CIRCLEDIALOG_H
