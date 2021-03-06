#include "endless_dialog.h"
#include "ui_endless_dialog.h"
#include <qmath.h>

Endless_Dialog::Endless_Dialog(QPointF startpoint, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Endless_Dialog)
{
    ui->setupUi(this);
     ui->forward_QR->setChecked(true); //  напрям, за  замовчування за годинковою стілкою
 startPoint=startpoint;
//задання обммеження для полів вводу
     QIntValidator *intValidator= new QIntValidator(1,65000,this);
     QIntValidator *coordValidator= new QIntValidator(-65000,65000,this);
      QIntValidator *AngleIntValidator= new QIntValidator(0,360,this);
ui->center_x_edit->setValidator(coordValidator);
ui->center_y_edit->setValidator(coordValidator);
ui->Height_D1_edit->setValidator(intValidator);
ui->Height_D2_edit->setValidator(intValidator);
ui->Angle_edit->setValidator(AngleIntValidator);
 //
 wSize = 200;  // розмір відображення
Angle = 360-45; // початковий кут 90 г ( у ЙТ і задачі різні напрями кутів)

Distance1=200; // рпосаткова дистанія
Distance2=200;
centerX=0;
centerY=0;
 setXY= false;

 painter = new QGraphicsScene(); // створюэмо головний елемент сцени
 ui->graphicsView->setScene(painter);
 ui->graphicsView->setMinimumSize(400,400);
 painter->setSceneRect(-50,-350,400,50);
 mainPaiting();
//

connect(ui->center_x_edit,SIGNAL(editingFinished()),this,SLOT(changedCenterX()));
connect(ui->center_y_edit,SIGNAL(editingFinished()),this,SLOT(changedCenterY()));
connect(ui->Angle_edit,SIGNAL(editingFinished()),this,SLOT(changedAngle()));
connect(ui->Height_D1_edit,SIGNAL(editingFinished()),this,SLOT(changedDistance1()));
 connect(ui->Height_D2_edit,SIGNAL(editingFinished()),this,SLOT(changedDistance2()));
 connect(ui->forward_QR,SIGNAL(toggled(bool)),this,SLOT(changedDirection()));
 connect(ui->becward_QR,SIGNAL(toggled(bool)),this,SLOT(changedDirection()));
 connect(ui->geo_coord_radioB,&QRadioButton::toggled,this,&Endless_Dialog::coordChange);
 connect(ui->local_coord_radioB,&QRadioButton::toggled,this,&Endless_Dialog::coordChange);
ui->local_coord_radioB->setChecked(true);

}

// стилі малювання


//основний метод малювання
void Endless_Dialog::mainPaiting(){
 // стилі малювання
    painter->clear();
 QPen pen1(Qt::red, 6, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
 QPen pen2(Qt::black,3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
 QPen TextPen(Qt::black,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
 QFont timesFont("Times", 10); //задаємо шрифт
QPainterPath arc;  // змінна для кола
QPointF arcX;  // точка кординат дуги
double rad = 8;  // розмыр точок
QPainterPath path2; // змінна для к ліній

qreal kordinadYr; // корината для малювання направляючої

double arrowAngle  = 0;   // кут стрілок
QPainterPath textPath; // змінна для тексту  ліній
QPainterPath textPathD;// змінна для тексту  ліній
 QPainterPath Line_D;// змінні для  лінії підпису
 QPainterPath Line_D1; // змінні для  лінії підпису
  QPainterPath Line_D2; // змінні для  лінії підпису
group_1 = new QGraphicsItemGroup(); // група для выдображення точок
QGraphicsItemGroup *pointEnum =new QGraphicsItemGroup[14];
group_Point = new QGraphicsItemGroup(); // група для выдображення точок
group_Dline = new  QGraphicsItemGroup(); // група для выдображення лінії підпису
QTransform transform; // трасформіція стрілок
QTransform transform2;// трансформація лінії підпиму
QPointF PointN;
QPointF Point1;  // точки прямокутника
QPointF Point2; // точки прямокутника
QPointF Point3;// точки прямокутника
QPointF Point4;// точки прямокутника

QVector<QPointF> PointVector; // вектор кординат точкоч

// малювання стрілок
QPolygon polygon;
polygon << QPoint( 12,  -5 );
polygon << QPoint( 0, 0 );
polygon << QPoint( 12,  5 );

//малюємо вертикальну напрвляючу
path2.moveTo(0,-400);
path2.lineTo(0,0);


// тангес кута
X_height = tan(Angle*M_PI/180);

// якщо кут в Йт у 1 і 2 четвертях кординат то напрямм малювання лінії додатній
if((Angle >=1 && Angle < 90) ||(Angle >270 && Angle < 360)){
    kordinadYr = X_height * 400  ;
        path2.lineTo(400, kordinadYr);
}

// інікше відємний
else{
   if(Angle >90 && Angle < 270 ){
       kordinadYr = X_height * -200  ;
           path2.lineTo(-200, kordinadYr);
   }
   // для перхідно значення 0 (90 на екрані)
        if(Angle == 0  ){
        path2.lineTo(400, 0);

    }
}
// малювання дуги піпису кута
arc.moveTo(0,0);
if (Angle > 270){
arc.arcTo(-20,-20,40,40,90,-(-270+Angle));
}
else
{
arc.arcTo(-20,-20,40,40,90,-(Angle+90));
}

// визначення кінцевої позиції для малювання стрілки
arcX = arc.currentPosition();
kordinadX= arcX.x();
kordinadY = arcX.y();
// тест підпису
textPath.addText(20 ,0, timesFont,tr("α"));

   transform.translate(kordinadX,kordinadY); // перемыщення в кынець ліній
    pointEnum[0].addToGroup(painter->addPolygon(polygon)); // додання до масиву
    pointEnum[0].setTransform(transform);
    pointEnum[0].setRotation( 220);  // поворот на кут
group_Point->addToGroup(&pointEnum[0]);  // додаання до групи відображення
 transform.translate(-kordinadX,-kordinadY);


// задання точок  прямокутника

 Point1.setX(-100);
 Point1.setY(-100);
 Point2.setX(100);
 Point2.setY(-100);
 Point3.setX(100);
 Point3.setY(100);
 Point4.setX(-100);
 Point4.setY(100);

 // нарпям за годинокою стрілкою
 if     (ui->forward_QR->isChecked()== true) {

     //точка1
        PointVector.append(Point1);
        PointVector.append(Point3);
        PointVector.append(Point2);
        PointVector.append(Point4);
        PointVector.append(Point1);

 }
 // нарпям проти годинокової стрілки
 else
 {
     PointVector.append(Point4);
     PointVector.append(Point2);
     PointVector.append(Point3);
     PointVector.append(Point1);
     PointVector.append(Point4);


 }
// точки стрілок підпису


PointN.setX(Point1.x());
PointN.setY(-130);
PointVector.append(PointN);
PointN.setX(Point2.x());
PointN.setY(-130);
PointVector.append(PointN);
PointN.setX(130);
PointN.setY(Point2.y());
PointVector.append(PointN);
PointN.setX(130);
PointN.setY(Point3.y());
PointVector.append(PointN);


// стрілки ліній  і лінії прчмокутника
            for (int i=0;i<4;i++){
             arrowAngle = atan2(PointVector[i].y() - PointVector[i+1].y(),PointVector[i].x()-PointVector[i+1].x() );
              transform.translate(PointVector[i+1].x(),PointVector[i+1].y()); // перемыщення в кынець ліній
               pointEnum[i+1].addToGroup(painter->addPolygon(polygon,pen1)); // додання до масиву
               pointEnum[i+1].setTransform(transform);
               pointEnum[i+1].setRotation(180/M_PI * arrowAngle);  // поворот на кут
               group_1->addToGroup(&pointEnum[i+1]);  // додаання до групи відображення
            transform.translate(-(PointVector[i+1].x()), -(PointVector[i+1].y()));
            // точки в вершинах прямокутника
        // лінії
            group_1->addToGroup(painter->addLine(PointVector[i].x(),PointVector[i].y(),PointVector[i+1].x(),PointVector[i+1].y(),pen1));
   // точки в вершинах прямокутника
            group_1->addToGroup(painter->addEllipse(PointVector[i].x()-rad, PointVector[i].y()-rad, rad*2.0, rad*2.0,QPen(), QBrush(Qt::SolidPattern)));
    // піпис точок
            textPathD.addText(PointVector[i].x()+10 ,PointVector[i].y()-10, timesFont,tr("ХТТ %1").arg(i+1));
            }


// стрілка  ліній підпису
arrowAngle=0;
for (int i=5;i<9;i++){
         //arrowAngle = atan2(PointVector[i].y() - PointVector[i+1].y(),PointVector[i].x()-PointVector[i+1].x() );
          transform.translate(PointVector[i].x(),PointVector[i].y()); // перемыщення в кынець ліній
           pointEnum[i+1].addToGroup(painter->addPolygon(polygon,pen2)); // додання до масиву
           pointEnum[i+1].setTransform(transform);
           pointEnum[i+1].setRotation( arrowAngle);  // поворот на кут
           group_1->addToGroup(&pointEnum[i+1]);  // додаання до групи відображення
        transform.translate(-(PointVector[i].x()), -(PointVector[i].y()));
        arrowAngle +=180; // кут стрілок
        if (i == 6){
        arrowAngle = 90;
        }

}


// малювання  боквих ліній D1
// малюємо в кординатах 0 0
Line_D1.moveTo(Point1.x(),Point1.y());
Line_D1.lineTo(Point1.x(),Point1.y()-50);

Line_D2.moveTo(Point2.x(),Point2.y());
Line_D2.lineTo(Point2.x(),Point2.y()-50);
// малювання   лінії D1
Line_D.moveTo(Point1.x(),Point1.y()-30);
Line_D.lineTo(Point2.x(),Point2.y()-30);
textPathD.addText(-20 ,-140, timesFont,tr("D1"));

//
group_1->addToGroup(painter->addPath(Line_D));
group_1->addToGroup(painter->addPath(Line_D1));
group_1->addToGroup(painter->addPath(Line_D2));

// точка центру
group_1->addToGroup(painter->addEllipse(0-rad, 0-rad, rad*2.0, rad*2.0,QPen(), QBrush(Qt::SolidPattern)));
textPathD.addText(10 ,15, timesFont,tr("Aц"));


// малювання   стілки лінії  D


    // малювання   лінії D 2

  Line_D1.moveTo(Point2.x(),Point2.y());
  Line_D1.lineTo(Point2.x()+50,Point2.y());

  Line_D2.moveTo(Point3.x(),Point3.y());
  Line_D2.lineTo(Point3.x()+50,Point3.y());
// малювання   лінії D
  Line_D.moveTo(130, Point2.y());
  Line_D.lineTo(130, Point3.y());
  textPathD.addText(140 , 0, timesFont,tr("D2"));

//
group_1->addToGroup(painter->addPath(Line_D));
group_1->addToGroup(painter->addPath(Line_D1));
group_1->addToGroup(painter->addPath(Line_D2));
group_1->addToGroup(painter->addPath(textPathD,TextPen));




// тарнформація на меобхідні кординати
transform2.translate( 150 , -150);
//group_Dline->setTransform(transform2);
transform2.rotate(Angle);
group_1->setTransform(transform2);


// додоємо до обєкта відображення
 painter->addItem(group_1);
painter->addItem(group_Point);

painter->addPath(path2);
painter->addPath(arc);
painter->addPath(textPath,TextPen);

}



//

//точки Х центру кола
void Endless_Dialog::changedCenterX(){

 centerX = ui->center_x_edit->text().toDouble();
  setXY= true;

}
//точки Y центру кола
void Endless_Dialog::changedCenterY(){
 centerY = ui->center_y_edit->text().toDouble();
 setXY= true;
}

void Endless_Dialog::coordChange()
{
    QLocale locale(QLocale::English);
    QDoubleValidator* validatorX =  new QDoubleValidator( -180, 180, 8 );
    QDoubleValidator* validatorY =  new QDoubleValidator( -90, 90, 8 );
     QIntValidator* intvalidatorX= new QIntValidator(-10000,10000);
     validatorX->setNotation(QDoubleValidator::StandardNotation);
     validatorY->setNotation(QDoubleValidator::StandardNotation);
     locale.setNumberOptions(QLocale::RejectGroupSeparator);
      validatorX->setLocale(locale);\
      validatorY->setLocale(locale);
    if(ui->geo_coord_radioB->isChecked()==true){

     ui->center_x_edit->setValidator(validatorX);
     ui->center_y_edit->setValidator(validatorY);
     ui->x_edit->setText("Довгота градуси");
     ui->y_edit->setText("Широта градуси");
    }
    else {

         ui->center_y_edit->setValidator(intvalidatorX);
         ui->center_x_edit->setValidator(intvalidatorX);
         ui->x_edit->setText("Локальний Х м");
         ui->y_edit->setText("Локальний Y м");
    }
}


// зчитування кута  з вікна
void Endless_Dialog::changedAngle(){
  Angle = ui->Angle_edit->text().toInt()  ;

 // первід в систему відображення ( зміщення на 90 градусів)
 if(Angle < 90){
     Angle = 360 -(90-Angle);
 }
 else
 {
     Angle = Angle -90;
 }

 //mainPaiting();

}
//
QVector<PointOfTrace> Endless_Dialog::getResPath(){

    QPainterPath ResPath;
   QVector<PointOfTrace> resPoints;
    //малювання вихыдноъ фыгури
    ResPath.moveTo(-Distance1/2 ,-Distance2/2); // poin1
   ResPath.lineTo(Distance1/2,Distance2/2); // point2
    ResPath.lineTo( Distance1/2,-Distance2/2);  // point3
     ResPath.lineTo(-Distance1/2,Distance2/2);// point4
       ResPath.lineTo(-Distance1/2 ,-Distance2/2);
    if     (ui->forward_QR->isChecked()== false) {
        ResPath = ResPath.toReversed(); // зміна напрямку руху.
    }

ResPath.translate(Distance1/2,0);
    QTransform transform1;
    transform1.rotate(Angle);
    //трансформуємо в нову лінію
    ResPath = transform1.map(ResPath);

    QPointF transPoint(centerX,centerY);
    if(ui->geo_coord_radioB->isChecked()==true && setXY){
        QPointF point1 (centerX,centerY);
        transPoint = returnStrateCoord(startPoint,point1);
    }
    ResPath.translate(transPoint.x()-Distance1/2,transPoint.y());



    for(int i = 0; i <  ResPath.elementCount(); i++)
    {
        QPointF pt = ResPath.elementAt(i); //отримуэмо точку лінії в і позиції
        pt = returnGeoCoord(startPoint, pt.x(),pt.y());
        PointOfTrace PointOfTracer(pt.x(),pt.y(),startPoint);
        resPoints.append(PointOfTracer);
     }
    return resPoints;

}
//
qreal Endless_Dialog::getAngle(){
    return Angle;
}

// зміна відстані 1
void Endless_Dialog::changedDistance1(){
    Distance1 = ui->Height_D1_edit->text().toInt();

}
 // зміна відстані 2
void Endless_Dialog::changedDistance2(){
    Distance2 = ui->Height_D2_edit->text().toInt();

}
// зміна панряму
void Endless_Dialog::changedDirection(){
    mainPaiting();
}
Endless_Dialog::~Endless_Dialog()
{
    delete ui;
}

bool Endless_Dialog::getSetXY(){
    return  setXY;
}
