#include "snake_dialog.h"
#include "ui_snake_dialog.h"
#include <qmath.h>

Snake_Dialog::Snake_Dialog(QPointF startpoint, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Snake_Dialog)
{
    ui->setupUi(this);
    ui->forward_QR->setChecked(true);
     startPoint=startpoint;
    //задання обммеження для полів вводу
    QIntValidator *intValidator= new QIntValidator(1,65000,this);
    QIntValidator *coordValidator= new QIntValidator(-65000,65000,this);
     QIntValidator *AngleIntValidator= new QIntValidator(0,360,this);
    ui->center_x_edit->setValidator(coordValidator);
    ui->center_y_edit->setValidator(coordValidator);
    ui->Height_D_edit->setValidator(intValidator);
    ui->Aplitude_edit->setValidator(intValidator);
    ui->cros_number_edit->setValidator(intValidator);
    ui->repeat_number_edit->setValidator(intValidator);
    ui->Angle_edit->setValidator(AngleIntValidator);


     //
    // wSize = 200;  // розмір відображення
    Angle = 360-45; // початковий кут 90 г ( у ЙТ і задачі різні напрями кутів)

    Distance=200; // рпосаткова дистанія
    centerX=0;
    centerY=0;
    RepeatNumber =1;
    CrossNumber =2;
    Aplitude =100;
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
    connect(ui->Height_D_edit,SIGNAL(editingFinished()),this,SLOT(changedDistance()));
    connect(ui->becward_QR,SIGNAL(toggled(bool)),this,SLOT(changedDirection()));
    connect(ui->forward_QR,SIGNAL(toggled(bool)),this,SLOT(changedDirection()));
    connect(ui->Aplitude_edit,SIGNAL(editingFinished()),this,SLOT(changedAplitude()));
    connect(ui->cros_number_edit,SIGNAL(editingFinished()),this,SLOT(changedCrossNumber()));
     connect(ui->repeat_number_edit,SIGNAL(editingFinished()),this,SLOT(changedRepeatNumber()));
     connect(ui->geo_coord_radioB,&QRadioButton::toggled,this,&Snake_Dialog::coordChange);
     connect(ui->local_coord_radioB,&QRadioButton::toggled,this,&Snake_Dialog::coordChange);
    ui->local_coord_radioB->setChecked(true);

    }



    //основний метод малювання
    void Snake_Dialog::mainPaiting(){
     // стилі малювання
     QPen pen1(Qt::red, 6, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
     QPen pen2(Qt::black,3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
      QPen TextPen(Qt::black,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
     QFont timesFont("Times", 10); //задаємо шрифт
  painter->clear();
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
    QPointF PointE ; //


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

     Point1.setX(25);
     Point1.setY(0);
     Point2.setX(50);
     Point2.setY(-50);
     Point3.setX(100);
     Point3.setY(50);
     Point4.setX(150);
     Point4.setY(-50);

     // нарпям за годинокою стрілкою
     if     (ui->forward_QR->isChecked()== true) {

         //точка1
            PointVector.append(Point1);
            PointVector.append(Point2);
            PointVector.append(Point3);
            PointVector.append(Point4);
            PointE.setX(Point4.x()+50);
            PointE.setY(Point3.y());
            PointVector.append(PointE);
            PointE.setX(PointE.x()+25);
            PointE.setY(0);
            PointVector.append(PointE);
            // вивид тесту початку фыгури
            textPathD.addText(PointVector[0].x()-20 ,PointVector[0].y()+30, timesFont,tr("ХТТ поч"));

     }
     // нарпям проти годинокової стрілки
     else
     {
         PointVector.append(Point1);

         PointE.setX(Point1.x()+25);
         PointE.setY(Point1.y()+50);
         PointVector.append(PointE);

         PointE.setX(PointE.x()+50);
         PointE.setY(PointE.y()-100);
         PointVector.append(PointE);

         PointE.setX(PointE.x()+50);
         PointE.setY(PointE.y()+100);
         PointVector.append(PointE);

         PointE.setX(PointE.x()+50);
         PointE.setY(PointE.y()-100);
         PointVector.append(PointE);

         PointE.setX(PointE.x()+25);
         PointE.setY(0);
         PointVector.append(PointE);
 // вивид тесту початку фыгури
        textPathD.addText(PointVector[0].x()-5 ,PointVector[0].y()-15, timesFont,tr("ХТТ поч"));
     }
    // точки стрілок підпису



    PointN.setX(Point1.x());
    PointN.setY(Point1.y()-105);
    PointVector.append(PointN);
    PointN.setX(PointE.x());
    PointN.setY(PointE.y()-105);
    PointVector.append(PointN);
    PointN.setX(PointVector[3].x()+140);
    PointN.setY(PointVector[3].y());
    PointVector.append(PointN);
    PointN.setX(PointVector[3].x()+140);
    PointN.setY(PointVector[4].y());
    PointVector.append(PointN);




    group_1->addToGroup(painter->addEllipse(PointVector[0].x()-rad*1.5, PointVector[0].y()-rad*1.5, rad*3.0, rad*3.0,QPen(), QBrush(Qt::SolidPattern)));
   //textPathD.addText(PointVector[0].x()-20 ,PointVector[0].y()+30, timesFont,tr("ХТТ поч"));
    // стрілки ліній  і лінії прчмокутника
                for (int i=0;i<5;i++){
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
                group_1->addToGroup(painter->addEllipse(PointVector[i+1].x()-rad, PointVector[i+1].y()-rad, rad*2.0, rad*2.0,QPen(), QBrush(Qt::SolidPattern)));
        // піпис точок
                textPathD.addText(PointVector[i+1].x()+5 ,PointVector[i+1].y() -10, timesFont,tr("ХТТ N%1").arg(i+2));
                }


    // стрілка  ліній підпису
    arrowAngle=0;
    for (int i=6;i<10;i++){
             //arrowAngle = atan2(PointVector[i].y() - PointVector[i+1].y(),PointVector[i].x()-PointVector[i+1].x() );
              transform.translate(PointVector[i].x(),PointVector[i].y()); // перемыщення в кынець ліній
               pointEnum[i+1].addToGroup(painter->addPolygon(polygon,pen2)); // додання до масиву
               pointEnum[i+1].setTransform(transform);
               pointEnum[i+1].setRotation( arrowAngle);  // поворот на кут
               group_1->addToGroup(&pointEnum[i+1]);  // додаання до групи відображення
            transform.translate(-(PointVector[i].x()), -(PointVector[i].y()));
            arrowAngle +=180; // кут стрілок
            if (i == 7){
                if (ui->forward_QR->isChecked()== true){
            arrowAngle = 90;
                } else {

                     arrowAngle = 270;
                }
            }

    }


    // малювання  боквих ліній D1
    // малюємо в кординатах 0 0
    Line_D1.moveTo(Point1.x(),Point1.y());
    Line_D1.lineTo(Point1.x(),Point1.y()-120);

    Line_D2.moveTo(PointE.x(),PointE.y());
    Line_D2.lineTo(PointE.x(),PointE.y()-120);
    // малювання   лінії L
    Line_D.moveTo(Point1.x(),Point1.y()-105);
    Line_D.lineTo(PointE.x(),PointE.y()-105);
    textPathD.addText(PointE.x() /2 ,-120, timesFont,tr("L"));

    //
    group_1->addToGroup(painter->addPath(Line_D));
    group_1->addToGroup(painter->addPath(Line_D1));
    group_1->addToGroup(painter->addPath(Line_D2));




    // малювання   стілки лінії  D


        // малювання   лінії D 2

      Line_D1.moveTo(PointVector[3].x(),PointVector[3].y());
      Line_D1.lineTo(PointVector[3].x()+150,PointVector[3].y());

      Line_D2.moveTo(PointVector[3].x()+50,PointVector[4].y());
      Line_D2.lineTo(PointVector[3].x()+150,PointVector[4].y());
    // малювання   лінії D
      Line_D.moveTo(PointVector[3].x()+140, PointVector[3].y());
      Line_D.lineTo(PointVector[3].x()+140, PointVector[4].y());
      textPathD.addText(PointVector[3].x()+150 , 20, timesFont,tr("D"));

    //
    group_1->addToGroup(painter->addPath(Line_D));
    group_1->addToGroup(painter->addPath(Line_D1));
    group_1->addToGroup(painter->addPath(Line_D2));
    group_1->addToGroup(painter->addPath(textPathD,TextPen));

    // тарнформація на меобхідні кординати
    transform2.translate( 50 , -50);
    transform2.rotate(Angle);
    group_1->setTransform(transform2);


    // додоємо до обєкта відображення
     painter->addItem(group_1);
    painter->addItem(group_Point);

    painter->addPath(path2);
    painter->addPath(arc);
    painter->addPath(textPath,TextPen);

    }




    //точки Х центру кола
    void Snake_Dialog::changedCenterX(){

     centerX = ui->center_x_edit->text().toDouble();
      setXY= true;

    }
    //точки Y центру кола
    void Snake_Dialog::changedCenterY(){
     centerY = ui->center_y_edit->text().toDouble();
      setXY= true;

    }

    //
    void Snake_Dialog::changedDistance(){
     Distance = ui->Height_D_edit->text().toInt();

    }
    // зчитування кута  з вікна
    void Snake_Dialog::changedAngle(){
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
void Snake_Dialog::changedAplitude(){

Aplitude = ui->Aplitude_edit->text().toInt();
}

//
void Snake_Dialog::changedCrossNumber(){
    CrossNumber = ui->cros_number_edit->text().toInt();

}

void Snake_Dialog::changedRepeatNumber(){
    RepeatNumber = ui->repeat_number_edit->text().toInt();

}

void Snake_Dialog::coordChange()
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
QVector<PointOfTrace> Snake_Dialog::getResPath(){
        QPainterPath ResPath;
       QVector<PointOfTrace> resPoints;
         QPointF PointR1 ; //
         QPointF PointR2 ; //

         int poinDistance = Distance /CrossNumber;
         ResPath = QPainterPath();
        PointR1.setX(0);
        PointR1.setY(0);

        ResPath.moveTo(0,0);

        for (int i=0;i<CrossNumber*RepeatNumber;i++){
            if (i ==0 ) {
                PointR2.setX(PointR1.x()+poinDistance/4);
                PointR2.setY(-Aplitude/2);
            }
            else{
            PointR2.setX(PointR1.x()+poinDistance/2);
            PointR2.setY(-Aplitude/2);
            }
            //resPointVector.append(PointR2);
            ResPath.lineTo(PointR2);
            PointR1.setX(PointR2.x()+poinDistance /2);
            PointR1.setY(+Aplitude/2);
            //resPointVector.append(PointR1);
            ResPath.lineTo(PointR1);
        }
        PointR2.setX(PointR1.x()+poinDistance/4);
        PointR2.setY(0);
        ResPath.lineTo(PointR2);



         if     (ui->forward_QR->isChecked()== false) {
             ResPath = ResPath.toReversed(); // зміна напрямку руху.
         }

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
    qreal Snake_Dialog::getAngle(){
        return Angle;
    }
    //
    void Snake_Dialog::changedDirection(){
        mainPaiting();
    }

Snake_Dialog::~Snake_Dialog()
{
    delete ui;
}
bool Snake_Dialog::getSetXY(){
    return  setXY;
}
