#include "line_dialog.h"
#include "ui_line_dialog.h"
#include <QGraphicsItemGroup>
#include <qmath.h>
#include "stratecoord.h"

Line_Dialog::Line_Dialog(QPointF startpoint, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Line_Dialog)
{
    ui->setupUi(this);
 startPoint=startpoint;
    //задання обммеження для полів вводу
    QIntValidator *intValidator= new QIntValidator(1,65000,this); 
    QIntValidator *coordValidator= new QIntValidator(-65000,65000,this);
     QIntValidator *AngleIntValidator= new QIntValidator(0,360,this);
    ui->center_x_edit->setValidator(coordValidator);
    ui->center_y_edit->setValidator(coordValidator);
    ui->Height_D_edit->setValidator(intValidator);
    ui->Angle_edit->setValidator(AngleIntValidator);
     //
     wSize = 200;  // розмір відображення
  Angle = 360-45; // початковий кут 90 г ( у ЙТ і задачі різні напрями кутів)

    Distance=100; // рпосаткова дистанія
    centerX=0;
    centerY=0;
 setXY= false;
     mainPaiting();
    //

    connect(ui->center_x_edit,SIGNAL(editingFinished()),this,SLOT(changedCenterX()));
    connect(ui->center_y_edit,SIGNAL(editingFinished()),this,SLOT(changedCenterY()));
    connect(ui->Angle_edit,SIGNAL(editingFinished()),this,SLOT(changedAngle()));
    connect(ui->Height_D_edit,SIGNAL(editingFinished()),this,SLOT(changedDistance()));
    connect(ui->geo_coord_radioB,&QRadioButton::toggled,this,&Line_Dialog::coordChange);
    connect(ui->local_coord_radioB,&QRadioButton::toggled,this,&Line_Dialog::coordChange);
   ui->local_coord_radioB->setChecked(true);

 }

// стилі малювання


 //основний метод малювання
 void Line_Dialog::mainPaiting(){
     // стилі малювання
     QPen pen1(Qt::red, 6, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
     QPen pen2(Qt::black,3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
      QPen TextPen(Qt::black,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

     QFont timesFont("Times", 10); //задаємо шрифт
      painter = new QGraphicsScene(); // створюэмо головний елемент сцени
      ui->graphicsView->setScene(painter);
      // задаємо розмір сцени
      //ui->graphicsView->resize(350,300);
     // ui->graphicsView->setFixedSize(350,300);
      ui->graphicsView->setMinimumSize(300,300);
      painter->setSceneRect(-50,-250,300,100);
    QPainterPath arc;  // змінна для кола
    QPointF arcX;  // точка кординат дуги
    double rad = 8;  // розмыр точок
    QPainterPath path2; // змінна для к ліній
  // int kordinadX ;  // кордина точки дуги відображення кута
  // int kordinadY ;//  кордина точки дуги відображення кута
   int kordinadX1 ;   //початкова кордината X
    int kordinadY1  ; //додаткова кордината Y
    int kordinadX2; //додаткова кордината X
    int kordinadY2;//додаткова кордината Y
    //qreal X_height; // тангес кута
    //qreal Line_Height; // довжина лінії
  //  qreal   K_height_line; // довжина розрахуковоъ лыныъ
    qreal kordinadYr; // корината для малювання направляючої
    qreal arrow_width = wSize/20;   //  розмір стрілок
    double arrowAngle  = 0;   // кут стрілок
    QPainterPath textPath; // змінна для тексту  ліній
    QPainterPath textPathD;// змінна для тексту  ліній
    QPainterPath Line_M; // змінна для   говної лінії
     QPainterPath Line_D;// змінні для  лінії підпису
     QPainterPath Line_D1; // змінні для  лінії підпису
      QPainterPath Line_D2; // змінні для  лінії підпису
  group_1 = new QGraphicsItemGroup(); // група для выдображення точок
  QGraphicsItemGroup *pointEnum =new QGraphicsItemGroup[4];
  group_Point = new QGraphicsItemGroup(); // група для выдображення точок
  group_Dline = new  QGraphicsItemGroup(); // група для выдображення лінії підпису
  QTransform transform; // трасформіція стрілок
   QTransform transform2;// трансформація лінії підпиму

 // малювання стрілок
  QPolygon polygon;
   polygon << QPoint( arrow_width,  -arrow_width/2 );
   polygon << QPoint( 0, 0 );
   polygon << QPoint( arrow_width,  arrow_width/2 );

   //малюємо вертикальну напрвляючу
   path2.moveTo(0,-200);
   path2.lineTo(0,0);


   // тангес кута
   X_height = tan(Angle*M_PI/180);

   // якщо кут в Йт у 1 і 2 четвертях кординат то напрямм малювання лінії додатній
   if((Angle >=1 && Angle < 90) ||(Angle >270 && Angle < 360)){
        kordinadYr = X_height * 200  ;
            path2.lineTo(200, kordinadYr);
   }

   // інікше відємний
   else{
       if(Angle >90 && Angle < 270 ){
           kordinadYr = X_height * -200  ;
               path2.lineTo(-200, kordinadYr);
       }
       // для перхідно значення 0 (90 на екрані)
            if(Angle == 0  ){
            path2.lineTo(200, 0);

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
    group_1->addToGroup(&pointEnum[0]);  // додаання до групи відображення
     transform.translate(-kordinadX,-kordinadY);

    // малювання   говної лінії
     X_height=tan(Angle*M_PI/180);
     // якщо кут в Йт у 1 і 2 четвертях кординат то напрямм малювання лінії додатній
     if((Angle >1 && Angle <= 90) ||(Angle >270 && Angle < 360)){
         kordinadX1= 50;
        kordinadY1 =  X_height* kordinadX1;

        Line_M.moveTo(kordinadX1,kordinadY1);
        kordinadX2= 150;
       kordinadY2 = X_height * kordinadX2;
         Line_M.lineTo(kordinadX2,kordinadY2);
     }
     // інакше відємний
     else{
         if(Angle >90 && Angle < 270 ){
             kordinadX1= -50;
            kordinadY1 =  X_height* kordinadX1;

            Line_M.moveTo(kordinadX1,kordinadY1);
            kordinadX2= -150;
           kordinadY2 = X_height * kordinadX2;
             Line_M.lineTo(kordinadX2,kordinadY2);
         }
         else {
             kordinadX1= 50;
            kordinadY1 =  0;

            Line_M.moveTo(kordinadX1,kordinadY1);
            kordinadX2= 120;
           kordinadY2 = 0;
             Line_M.lineTo(kordinadX2,kordinadY2);
         }
     }
     // вівід результату


// довжина головної лінії
Line_Height=sqrt((kordinadX1-kordinadX2)*(kordinadX1-kordinadX2)+(kordinadY1-kordinadY2)*(kordinadY1-kordinadY2));


// стрілка говної лінії
     arrowAngle = atan2(kordinadY1-kordinadY2,kordinadX1-kordinadX2 );
      transform.translate(kordinadX2,kordinadY2); // перемыщення в кынець ліній
       pointEnum[1].addToGroup(painter->addPolygon(polygon,pen1)); // додання до масиву
       pointEnum[1].setTransform(transform);
       pointEnum[1].setRotation(180/M_PI * arrowAngle);  // поворот на кут
       group_1->addToGroup(&pointEnum[1]);  // додаання до групи відображення
transform.translate(-kordinadX2,-kordinadY2);

// малювання  точок
     group_Point->addToGroup(painter->addEllipse(kordinadX1-rad, kordinadY1-rad, rad*2.0, rad*2.0,QPen(), QBrush(Qt::SolidPattern)));
    group_Point->addToGroup(painter->addEllipse(kordinadX2 - rad, kordinadY2 - rad, rad*2.0, rad*2.0,QPen(), QBrush(Qt::SolidPattern)));
//
// малювання  боквих ліній D
// малюємо в кординатах 0 0
    Line_D1.moveTo(0,0);
    Line_D1.lineTo(0,20);

    Line_D2.moveTo(Line_Height,0);
    Line_D2.lineTo(Line_Height,20);
// малювання   лінії D
    Line_D.moveTo(0,15);
    Line_D.lineTo(Line_Height,15);
    textPathD.addText(Line_Height/2-30 ,40, timesFont,tr("Довжина L"));

//
group_Dline->addToGroup(painter->addPath(Line_D));
group_Dline->addToGroup(painter->addPath(Line_D1));
group_Dline->addToGroup(painter->addPath(Line_D2));
group_Dline->addToGroup(painter->addPath(textPathD,TextPen));


// малювання   стілки лінії  D

    arrowAngle = atan2(0, -Line_Height );
     transform.translate(Line_Height,15); // перемыщення в кынець ліній
      pointEnum[2].addToGroup(painter->addPolygon(polygon,pen2)); // додання до масиву
      pointEnum[2].setTransform(transform);
      pointEnum[2].setRotation(180/M_PI * arrowAngle);  // поворот на кут
      group_Dline->addToGroup(&pointEnum[2]);  // додаання до групи відображення
       transform.translate(-Line_Height,-15);
       arrowAngle = atan2(0, Line_Height );
       transform.translate(0,15); // перемыщення в кынець ліній
      // transform2.scale(1.5,1.5);
        pointEnum[3].addToGroup(painter->addPolygon(polygon,pen2)); // додання до масиву
        pointEnum[3].setTransform(transform);
        pointEnum[3].setRotation(180/M_PI * arrowAngle);  // поворот на кут


        group_Dline->addToGroup(&pointEnum[3]);  // додаання до групи відображення
      transform.translate(0,-15);

// перміщаємо до головної лінії, перевртаємо в залежності від кута говної лінії
       if((Angle >1 && Angle <= 90) ||(Angle >270 && Angle < 360)){
              transform2.translate( kordinadX1 , kordinadY1);
       transform2.rotate(Angle);
       }
       else
       {
           transform2.translate( kordinadX2 , kordinadY2);
       transform2.rotate(Angle+180);
       }
       group_Dline->setTransform(transform2);

// додоємо до обєкта відображення
     painter->addItem(group_1);
      painter->addItem(group_Point);
      painter->addItem(group_Dline);
 painter->addPath(path2);
   painter->addPath(arc);
    painter->addPath(textPath,TextPen);
   painter->addPath(Line_M,pen1);


 }

 //точки Х центру кола
 void Line_Dialog::changedCenterX(){

     centerX = ui->center_x_edit->text().toDouble();
      setXY= true;

 }
 //точки Y центру кола
 void Line_Dialog::changedCenterY(){
     centerY = ui->center_y_edit->text().toDouble();
      setXY= true;

 }

 void Line_Dialog::coordChange()
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

 //
 void Line_Dialog::changedDistance(){
     Distance = ui->Height_D_edit->text().toInt();

 }
 // зчитування кута  з вікна
 void Line_Dialog::changedAngle(){
     Angle = ui->Angle_edit->text().toInt()  ;

     // первід в систему відображення ( зміщення на 90 градусів)
     if(Angle < 90){
         Angle = 360 -(90-Angle);
     }
     else
     {
         Angle = Angle -90;
     }

    // mainPaiting();

 }
 //вивід точко резултату
 QVector<PointOfTrace> Line_Dialog::getResPoint(){

     QPainterPath ResPath;
    QVector<PointOfTrace> resPoints;
     //малювання вихыдноъ фыгури
     ResPath = QPainterPath();
    ResPath.moveTo(0,0);
   ResPath.lineTo(Distance,0);
   QTransform transform1;
       transform1.rotate(Angle);
       //трансформуємо в нову лінію
       ResPath = transform1.map(ResPath);
       QPointF transPoint(centerX,centerY);
       if(ui->geo_coord_radioB->isChecked()==true && setXY){
           QPointF point1 (centerX,centerY);
           transPoint = returnStrateCoord(startPoint,point1);
       }

ResPath.translate(transPoint.x(),transPoint.y());


    for(int i = 0; i <  ResPath.elementCount(); i++)
    {
        QPointF pt = ResPath.elementAt(i); //отримуэмо точку лінії в і позиції
        pt = returnGeoCoord(startPoint, pt.x(),pt.y());
        PointOfTrace PointOfTracer(pt.x(),pt.y(),startPoint);
        resPoints.append(PointOfTracer);
     }

    return resPoints;
 }

Line_Dialog::~Line_Dialog()
{
    delete ui;
}
bool Line_Dialog::getSetXY(){
    return  setXY;
}
