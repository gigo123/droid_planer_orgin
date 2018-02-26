#include "circledialog.h"
#include "ui_circledialog.h"
#include <QGraphicsItemGroup>
#include <qmath.h>

CircleDialog::CircleDialog(QPointF startpoint, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CircleDialog)
{

    ui->setupUi(this);
   ui->aproxim_Box->setRange(5,360);
   ui->pushButton_Ok->setEnabled(false);
   //задання обммеження для полів вводу
   QIntValidator *intValidator= new QIntValidator(1,65000,this);
   ui->radius_edit->setValidator(intValidator);
   //ui->cenrer_x_edit->setValidator(intValidator);
   //ui->center_y_edit->setValidator(intValidator);
    startPoint=startpoint; //
   ui->forward_QR->setChecked(true);
    ui->aproxim_Box->setWrapping(false);
    mDirection = -1;// початковий напрям
    aproximNu = 5; //  базова кількість ліній
    wSize = 200;  // розмір відображення
    Radius = 100;
    centerXR =100;
    centerYR =100;
     setXY= false;

    mainPaiting();
   //
   connect(ui->forward_QR,SIGNAL(toggled(bool)),this,SLOT(changedDirectionO()));
   connect(ui->becward_QR,SIGNAL(toggled(bool)),this,SLOT(changedDirectionR()));
   connect(ui->aproxim_Box,SIGNAL(valueChanged(int)),this,SLOT(changedAproximN()));
   connect(ui->radius_edit,SIGNAL(editingFinished()),this,SLOT(changedRadius()));
   connect(ui->center_x_edit,SIGNAL(editingFinished()),this,SLOT(changedCenterX()));
   connect(ui->center_y_edit,SIGNAL(editingFinished()),this,SLOT(changedCenterY()));
   connect(ui->pushButton_Ok,SIGNAL(pressed()),this,SLOT(accept()));
   connect(ui->pushButton_Cancel,SIGNAL(pressed()),this,SLOT(reject()));
   connect(ui->geo_coord_radioB,&QRadioButton::toggled,this,&CircleDialog::coordChange);
   connect(ui->local_coord_radioB,&QRadioButton::toggled,this,&CircleDialog::coordChange);

  ui->local_coord_radioB->setChecked(true);

}

CircleDialog::~CircleDialog()
{
    delete ui;
    //delete[] pointEnum;
}

//основний метод малювання
void CircleDialog::mainPaiting(){
    QPen pen1(Qt::red, 6, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
     QPen TextPen(Qt::black,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QFont timesFont("Times", 10); //задаємо шрифт
     painter = new QGraphicsScene(); // створюэмо головний елемент сцени
      ui->graphicsView->setScene(painter);
   QPainterPath ellipseP;  // змінна для кола
   double rad = 3;  // розмыр точок
   QPainterPath path2; // змінна для к ліній
   qreal kordinadX =0;  // початкова кордината X першої лінії, вхід в коло
   qreal kordinadY =wSize/2 ;// початкова кордината Y  першої лінії, вхід в коло
   qreal kordinadX1 =0;   //додаткова кордината X
    qreal kordinadY1  = wSize/2 ; //додаткова кордината Y
   qreal centerX = wSize/2; // кордината центра кола  ( Y=X)
   qreal arrow_width = wSize/20;   //  розмір стрілок
   double arrowAngle  = 0;   // кут стрілок
   QPainterPath textPath; // змінна для тексту  ліній
   QPainterPath radius; // змінна для   лінії радіусу
 group_1 = new QGraphicsItemGroup(); // група для выдображення точок
QGraphicsItemGroup *pointEnum =new QGraphicsItemGroup[aproximNu+1]; // масив для точок
 QTransform transform;
 qreal kordinadXR =0;  // початкова кордината X першої лінії, вхід в коло
 qreal kordinadYR =Radius ;// початкова кордината Y  першої лінії, вхід в коло
 qreal centerXR = Radius; // кордината центра кола  ( Y=X)
QPainterPath emptyPath;
 ResPath = emptyPath;
// малювання стрілок
 QPolygon polygon;
  polygon << QPoint( arrow_width,  -arrow_width/2 );
  polygon << QPoint( 0, 0 );
  polygon << QPoint( arrow_width,  arrow_width/2 );
   // малюємо квадрвт на всю полощину
QRect Rect1( 0,0,wSize,wSize);
// змынити painter->setPen(QPen(Qt::black, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
ellipseP.addEllipse(Rect1);// строрюємо коло вписене в кавдрат
textPath.addText(-30, wSize/2 -10, timesFont, tr("Поч"));
path2.moveTo(kordinadX,kordinadY);  // задаємо початкову точку
ResPath.moveTo(kordinadXR,kordinadYR);

//цикл малювання
// i=1 через наявніть почасткової точки, починаємо знаходження з другої
//i<aproximNu+1, еле кількість ліній повинна бути повною
for(int i=1;i<aproximNu+1;i++){
    // за домогою геометричних правил знаходимо кординати наступної  точки
    //останній парметр - початкова фаза
    kordinadX =centerX + centerX*cos(mDirection*2*M_PI *i/aproximNu + 3*M_PI);
    kordinadY = centerX - centerX*sin(mDirection*2*M_PI *i/aproximNu  + 3*M_PI);
        path2.lineTo(kordinadX ,kordinadY);

        kordinadXR =centerXR + centerXR*cos(mDirection*2*M_PI *i/aproximNu + 3*M_PI);
        kordinadYR = centerXR - centerXR*sin(mDirection*2*M_PI *i/aproximNu  + 3*M_PI);
            ResPath.lineTo(kordinadXR ,kordinadYR);


//задаємо параметри відображення точок
       painter->addEllipse(kordinadX-rad, kordinadY-rad, rad*2.0, rad*2.0, QPen(), QBrush(Qt::SolidPattern));

   // кут стрілок
      arrowAngle = atan2( kordinadY1-kordinadY,kordinadX1-kordinadX );
      transform.translate(kordinadX,kordinadY); // перемыщення в кынець ліній
       pointEnum[i].addToGroup(painter->addPolygon(polygon,pen1)); // додання до масиву
       pointEnum[i].setTransform(transform);
       pointEnum[i].setRotation(180/M_PI * arrowAngle);  // поворот на кут
       group_1->addToGroup(&pointEnum[i]);  // додаання до групи відображення
        transform.translate(-kordinadX,-kordinadY);


  if(i==aproximNu-1){
// кінцева точка
   textPath.addText(kordinadX -20.0 ,kordinadY + 25.0, timesFont,tr("Кін"));
   }
   //  маллювання радісу і точки ценра кола
   if(i==1){

    radius.moveTo(centerX,centerX);
    radius.lineTo(kordinadX,kordinadY);
   //   painter.drawPath(radius);
    painter->addPath(radius);
      // малювання стрілок
         arrowAngle = atan2( centerX-kordinadY,centerX- kordinadX );
         transform.translate(kordinadX,kordinadY);
         pointEnum[0].addToGroup(painter->addPolygon(polygon,pen1));
         pointEnum[0].setTransform(transform);
         pointEnum[0].setRotation(180/M_PI * arrowAngle);
         group_1->addToGroup(&pointEnum[0]);
          transform.translate(-kordinadX,-kordinadY);

      painter->addEllipse(centerX-rad, centerX-rad, rad*2.0, rad*2.0,QPen(), QBrush(Qt::SolidPattern));

         // підписи
       textPath.addText(centerX -20.0 ,centerX + 25.0, timesFont,tr("Aц"));
       textPath.addText((centerX +kordinadX)/2 + 15 ,(centerX + kordinadY)/2 , timesFont,tr("R"));
   }



kordinadX1=kordinadX;
kordinadY1=kordinadY;


}
//додавання до графічного обєкта
painter->addItem(group_1);
painter->addPath(path2);
painter->addPath(ellipseP);
painter->addPath(textPath,TextPen);

  // ui->graphicsView->setScene(painter);// вивід на екран
   //delete[] pointEnum;
}
//напрям за годинноковою стіолкою
void CircleDialog::changedDirectionO(){
    mDirection = -1;
    mainPaiting();
}
// протти годиннкової стрілки
void CircleDialog::changedDirectionR(){
    mDirection = 1;
    mainPaiting();
}
// зміна кількості точок апроксимації
void CircleDialog::changedAproximN(){
    aproximNu  = ui->aproxim_Box->value();
    mainPaiting();
}

// зміна радіусу
void CircleDialog::changedRadius(){
 //   int Radius1 =  Radius;
    Radius = ui->radius_edit->text().toInt();
  //  if(Radius !=0 && Radius !=Radius1){
        ui->pushButton_Ok->setEnabled(true);
 //  }
    mainPaiting();

}
//точки Х центру кола
void CircleDialog::changedCenterX(){
  //  int centerX1 =centerXR;
    centerX = ui->center_x_edit->text().toDouble();
  //  if(centerXR !=0 && centerXR!=centerX1){
 ui->pushButton_Ok->setEnabled(true);
//    }
  setXY= true;
}
//точки Y центру кола
void CircleDialog::changedCenterY(){
//int centerY1 =centerYR;
    centerY = ui->center_y_edit->text().toDouble();
   // if(centerYR !=0 && centerYR!=centerY1){
     ui->pushButton_Ok->setEnabled(true);
   // }
     setXY= true;
}

void CircleDialog::coordChange()
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
/*QVector<int> CircleDialog::getResPoint(){

   return Respoint;
}
*/
//
QVector<PointOfTrace>  CircleDialog::getResPath(){
    QVector<PointOfTrace> resPoints;

    QPointF transPoint(centerX,centerY);
    if(ui->geo_coord_radioB->isChecked()==true && setXY){
        QPointF point1 (centerX,centerY);
        transPoint = returnStrateCoord(startPoint,point1);
    }

    ResPath.translate(-Radius +  transPoint.x(),-Radius+  transPoint.y());

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
bool CircleDialog::getSetXY(){
      return setXY;
}
