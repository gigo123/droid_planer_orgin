#ifndef MANEVR_ITEM_H
#define MANEVR_ITEM_H

#include <QObject>
#include <QPainterPath>
#include "stratecoord.h"
#include "pointt.h"

class manevr_Item
{
public:

      manevr_Item(QVector<PointOfTrace> PointNumbers, QString name, QPointF starpoint);
      manevr_Item();
   QVector<PointOfTrace> getPointsNumber();
//   QVector<QPointF> getPointsNumberlocal();
   QVector<PointOfTrace> getSurveyNumber();
   bool hasSyrvey();
   QString ManevrName();
   void setManevName(QString manName);
   void setPointsNumber(QVector<PointOfTrace> newPointsNumber);
  // void setPointsNumberLocal(QVector<QPointF> newPointsNumber);
    void setSurveyNumber(QVector<PointOfTrace> surveyNumbers);
    QPointF getGlobalStartPoint();
    void setglobalStratPoint(QPointF poinstS);
    void setglobalStratPointStart(QPointF poinstS);
  // void getManevrName();
    int getSpeed();
    void setSpeed(int speeD);
    int getRepeatNumber();
    void setRepeatNumber(int num);

private:
   int startElementCount;
     QVector<PointOfTrace> pointNumber;
     QVector<PointOfTrace> surveyNumber;
     QVector<QPointF> pointNumberLocal;
     QPointF startPoint;
     bool isSyrvey;
      QString manevrName;
      int speed;
      int repeatNumber;

};

#endif // MANEVR_ITEM_H
