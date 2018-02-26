#include "stratecoord.h"
#include <QtMath>
#include <qmath.h>
strateCoord::strateCoord()
{

}
QPointF strateCoord::returnStrateCoord(QPointF pointStr, QPointF geoPoint){
    if(pointStr==geoPoint) return QPointF(0,0);

    double km =0;
    double a1 = pointStr.x()* (M_PI/180.0);
    double b1 = pointStr.y()* (M_PI/180.0);
    int dir= 1;
    QPointF pointRes(0,0);
    double a2 = geoPoint.x() *(M_PI/180.0);
    double b2 = pointStr.y()* (M_PI/180.0);
    double r = 6378;

    km = acos(cos(a1)*cos(b1)*cos(a2)*cos(b2) + cos(a1)*sin(b1)*cos(a2)*sin(b2) + sin(a1)*sin(a2)) * r;
    if(geoPoint.x()<pointStr.x()) dir = -1;
    pointRes.setX(km*1000*dir);
    dir =1;
     a2 = pointStr.x() *(M_PI/180.0);
     b2 = geoPoint.y()* (M_PI/180.0);
    km = acos(cos(a1)*cos(b1)*cos(a2)*cos(b2) + cos(a1)*sin(b1)*cos(a2)*sin(b2) + sin(a1)*sin(a2)) * r;
      if(geoPoint.y()<pointStr.y()) dir = -1;
    pointRes.setY(km*1000*dir*2);
    return pointRes;
}


QPointF strateCoord::returnGeoCoord(QPointF pointStr ,qreal coordX, qreal coordY){
    if(coordX==0 && coordY==0) return pointStr;
    double km =0;
    double a1 = pointStr.x()* (M_PI/180.0);
    double b1 = pointStr.y()* (M_PI/180.0);
    int negX=1;
    int negY=-1;
    if (coordX<0)  {
        negX=-1;
        coordX=coordX*-1;
    }
    if (coordY<0)  {
        negY=1;
        coordY=coordY*-1;
    }
    QPointF pointRes(pointStr.x()+0.00001*negX*coordX,pointStr.y()+0.00001*negY*coordY);
    double a2 = pointRes.x() *(M_PI/180.0);
    double b2 = pointRes.y() *(M_PI/180.0);
    double r = 6378;
        if(coordX!=0)
        {
    while(km<coordX/1000)  {
                km = acos(cos(a1)*cos(b1)*cos(a2)*cos(b2) + cos(a1)*sin(b1)*cos(a2)*sin(b2) + sin(a1)*sin(a2)) * r;
                pointRes.setX(pointRes.x()+0.00001*negX);
               a2 = pointRes.x()* (M_PI/180.0);
            }
        }
    km =0;
    a2=a1;
    if(coordY!=0)
    {
    while(km<coordY/1000){
               km = acos(cos(a1)*cos(b1)*cos(a2)*cos(b2) + cos(a1)*sin(b1)*cos(a2)*sin(b2) + sin(a1)*sin(a2)) * r;
               pointRes.setY(pointRes.y()+0.00001*negY);
              b2 = pointRes.y()* (M_PI/180.0);

            }
    if(negY==-1){
        pointRes.setY(pointStr.y()-(pointStr.y()-pointRes.y())/2);
    }
    else {
        pointRes.setY(pointStr.y()-(pointStr.y()-pointRes.y())/2);
    }
    }
    return pointRes;
}
