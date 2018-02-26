#ifndef PointOfTrace_H
#define PointOfTrace_H

#include <QObject>
#include "stratecoord.h"

class PointOfTrace : public QPointF, public strateCoord
{
public:
    PointOfTrace();
    PointOfTrace(qreal pointx, qreal pointy, QPointF startpoint, int number=0);
    void setPointG(QPointF geopoint);
    void setPointlocal(QPointF localpoint);
    QPointF getGeoPoint();
    QPointF getLocalPoint();
    void setAltitude(int alt);
    int  getAltitude();
    void setSpeed(int speed);
    int getSpeed();
    void setStartPoint(QPointF newPoint);

private:

    int altitude;
    int speeD;
  QPointF localPoint;
  QPointF geoPoint;
  QPointF startPoint;
    int numberInPatch;
};

#endif // PointOfTrace_H
