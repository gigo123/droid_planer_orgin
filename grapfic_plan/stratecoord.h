#ifndef STRATECOORD_H
#define STRATECOORD_H
#include <QPoint>

class strateCoord
{
public:
    strateCoord();
    QPointF returnStrateCoord(QPointF pointStr, QPointF geoPoint);
    QPointF returnGeoCoord(QPointF pointStr ,qreal coordX, qreal coordY);
  //  QPointF returnStratePoint(QPointF pointStr,qreal coordX, qreal coordY);
};

#endif // STRATECOORD_H
