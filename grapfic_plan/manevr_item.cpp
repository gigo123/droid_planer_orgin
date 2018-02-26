#include "manevr_item.h"
#include "stratecoord.h"

manevr_Item::manevr_Item(QVector<PointOfTrace> PointNumbers, QString name, QPointF starpoint)
{
    manevrName = name;
    pointNumber = PointNumbers;
    startPoint=starpoint;
repeatNumber =1;
isSyrvey= false;
}

manevr_Item::manevr_Item()
{

}

QVector<PointOfTrace> manevr_Item::getPointsNumber(){
    return pointNumber;
}



QString manevr_Item::ManevrName(){
    return manevrName;
}

void manevr_Item::setManevName(QString manName)
{
    manevrName= manName;
}
void manevr_Item::setPointsNumber(QVector<PointOfTrace> newPointsNumber){
    pointNumber=newPointsNumber;

}



QVector<PointOfTrace> manevr_Item::getSurveyNumber(){
    return surveyNumber;
}
bool manevr_Item::hasSyrvey(){
    return isSyrvey;
}


void manevr_Item::setSurveyNumber(QVector<PointOfTrace> surveyNumbers){

       surveyNumber = surveyNumbers;
        isSyrvey= true;


}

QPointF manevr_Item::getGlobalStartPoint()
{
    return startPoint;

}

void manevr_Item::setglobalStratPoint(QPointF poinstS)
{
    startPoint =poinstS;
    for(int i=0; i<pointNumber.size();i++) pointNumber[i].setStartPoint(startPoint);

}

void manevr_Item::setglobalStratPointStart(QPointF poinstS)
{
    startPoint =poinstS;
  pointNumber[0].setPointlocal(QPointF(0,0));

}

int manevr_Item::getSpeed()
{
    return speed;
}

void manevr_Item::setSpeed(int speeD)
{
    speed =speeD;

}

int manevr_Item::getRepeatNumber()
{
    return repeatNumber;
}

void manevr_Item::setRepeatNumber(int num)
{
    repeatNumber = num;
}
