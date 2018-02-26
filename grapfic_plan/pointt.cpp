#include "pointt.h"

PointOfTrace::PointOfTrace()
{

}

PointOfTrace::PointOfTrace(qreal pointx,qreal pointy, QPointF startpoint,int number): QPointF(pointx,pointy)
{
    startPoint =startpoint;
    geoPoint =  QPointF(pointx,pointy);
   localPoint = returnStrateCoord(startpoint,geoPoint);
}

void PointOfTrace::setPointG(QPointF geopoint)
{

    geoPoint = geopoint;
   localPoint= returnStrateCoord(startPoint,geoPoint);
   setX(geopoint.x());
   setY(geopoint.y());

}

void PointOfTrace::setPointlocal(QPointF localpoint)
{

   localPoint= localpoint;
   geoPoint = returnGeoCoord(startPoint,localpoint.x(),localpoint.y());
   setX(geoPoint.x());
   setY(geoPoint.y());

}

QPointF PointOfTrace::getGeoPoint()
{
    return geoPoint;
}

QPointF PointOfTrace::getLocalPoint()
{
    return localPoint;
}

void PointOfTrace::setAltitude(int alt)
{
    altitude =alt;

}

int PointOfTrace::getAltitude()
{
    return altitude;

}

void PointOfTrace::setSpeed(int speed)
{
    speeD = speed;

}

int PointOfTrace::getSpeed()
{
    return speeD;
}

void PointOfTrace::setStartPoint(QPointF newPoint)

{

        startPoint=newPoint;
        localPoint= returnStrateCoord(startPoint,geoPoint);

}
