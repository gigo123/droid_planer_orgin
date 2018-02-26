#include "region_dialog.h"
#include "ui_region_dialog.h"
#include <QMessageBox>
#include <qmath.h>

Region_Dialog::Region_Dialog(QPointF startpoint, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Region_Dialog)
{
    ui->setupUi(this);
     startPoint=startpoint;
//задання обммеження для полів вводу
QIntValidator *intValidator= new QIntValidator(1,65000,this);
QIntValidator *pointValidator= new QIntValidator(-20000,20000,this);
 QIntValidator *AngleIntValidator= new QIntValidator(0,360,this);
ui->center_x_edit->setValidator(pointValidator);
ui->center_y_edit->setValidator(pointValidator);
 ui->point_x_edit->setValidator(pointValidator);
  ui->point_y_edit->setValidator(pointValidator);
  ui->proces_distance_edit->setValidator(intValidator);
    ui->Angle_edit->setValidator(AngleIntValidator);
 //
 wSize = 200;  // розмір відображення
Angle = 360-45; // початковий кут 90 г ( у ЙТ і задачі різні напрями кутів)

Distance=200; // рпосаткова дистанія
//кординати не введені
 Poinchanged = false;


 setXY= false;

ui->delete_Point_Button->setEnabled(false);
//
ui->tableWidget->setColumnCount(2); //задємо кількість колонок
ui->tableWidget->setShowGrid(true); // вствновлюємо показ сітки
// можливість вибирати еленти тільки по одному
ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
// можлтвість вибирати тільки порядково
ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//
ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "X"<<"Y");
//розтягуэмо отснню колону на всю ширину
//ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
painter = new QGraphicsScene(); // створюэмо головний елемент сцени
ui->graphicsView->setScene(painter);
ui->graphicsView->setMinimumSize(580,500);
painter->setSceneRect(-20,-500,550,50);

 mainPaiting();
//

connect(ui->center_x_edit,SIGNAL(editingFinished()),this,SLOT(changedCenterX()));
connect(ui->center_y_edit,SIGNAL(editingFinished()),this,SLOT(changedCenterY()));
connect(ui->Angle_edit,SIGNAL(editingFinished()),this,SLOT(changedAngle()));
connect(ui->proces_distance_edit,SIGNAL(editingFinished()),this,SLOT(changedDistance()));
connect(ui->add_Point_Button,SIGNAL(pressed()),this,SLOT(add_point()));
connect(ui->delete_Point_Button,SIGNAL(pressed()),this,SLOT(removePointOfTraceable()));
connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&Region_Dialog::acceptR);
connect(ui->point_x_edit,&QLineEdit::editingFinished,[this]{Poinchanged = true;});
connect(ui->point_y_edit,&QLineEdit::editingFinished,[this]{Poinchanged = true;});
connect(ui->geo_coord_radioB,&QRadioButton::toggled,this,&Region_Dialog::coordChange);
connect(ui->local_coord_radioB,&QRadioButton::toggled,this,&Region_Dialog::coordChange);
ui->local_coord_radioB->setChecked(true);

}
        //основний метод малювання
  void Region_Dialog::mainPaiting(){
      painter->clear();
         // стилі малювання
         QPen pen1(Qt::red, 6, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
         QPen pen2(Qt::black,3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
         QPen regionPen(Qt::black,2, Qt::DashDotDotLine, Qt::RoundCap, Qt::RoundJoin);
          QPen TextPen(Qt::black,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
         QFont timesFont("Times", 10); //задаємо шрифт

        QPainterPath arc;  // змінна для кола
        QPointF arcX;  // точка кординат дуги
        double rad = 8;  // розмыр точок
        QPainterPath path2; // змінна для к ліній

        qreal kordinadYr; // корината для малювання направляючої
        qreal arrow_width = wSize/20;   //  розмір стрілок
        double arrowAngle  = 0;   // кут стрілок
        QPainterPath textPath; // змінна для тексту  ліній
        QPainterPath textPathD;// змінна для тексту  ліній
         QPainterPath Line_D;// змінні для  лінії підпису
         QPainterPath Line_D1; // змінні для  лінії підпису
          QPainterPath Line_D2; // змінні для  лінії підпису
        group_1 = new QGraphicsItemGroup(); // група для выдображення точок
        QGraphicsItemGroup *pointEnum =new QGraphicsItemGroup[20];
        group_Point = new QGraphicsItemGroup(); // група для выдображення точок
        group_Dline = new  QGraphicsItemGroup(); // група для выдображення лінії підпису
        QTransform transform; // трасформіція стрілок
        QTransform transform2;// трансформація лінії підпиму
        QPointF PointN; // змвнн для задання очок
        QPointF Point1;  // точки прямокутника
        QPointF Point2; // точки прямокутника
        QPointF Point3;// точки прямокутника
        QPointF Point4;// точки прямокутника
        QPointF Point5 ; //

        QVector<QPointF> PointVector; // вектор кординат точкоч

        // малювання стрілок
        QPolygon polygon;
        polygon << QPoint( arrow_width,  -5 );
        polygon << QPoint( 0, 0 );
        polygon << QPoint( arrow_width,  5 );

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


        // задання точок  області
        Point1.setX(0);
         Point1.setY(0);
         Point2.setX(150);
         Point2.setY(0);
         Point3.setX(300);
         Point3.setY(150);
         Point4.setX(180);
         Point4.setY(300);
         Point5.setX(0);
         Point5.setY(280);
              PointVector.append(Point1);
                PointVector.append(Point2);
                PointVector.append(Point3);
                PointVector.append(Point4);
                PointVector.append(Point5);
                PointVector.append(Point1);



        // лінія фігцри

                PointN.setX(-40);
                PointN.setY(20);
                 PointVector.append(PointN);
                 PointN.setX(200);
                 PointN.setY(20);
                  PointVector.append(PointN);
                  PointN.setX(330);
                  PointN.setY(130);
                   PointVector.append(PointN);
                   PointN.setX(-40);
                   PointN.setY(130);
                    PointVector.append(PointN);
                    PointN.setX(-40);
                    PointN.setY(240);
                     PointVector.append(PointN);
                    PointN.setX(250);
                    PointN.setY(240);
                     PointVector.append(PointN);

  // точки стрілок підпису
        PointN.setX(350);
        PointN.setY(20);
        PointVector.append(PointN);
        PointN.setX(350);
        PointN.setY(130);
        PointVector.append(PointN);
        PointN.setX(0);
        PointN.setY(260);
        PointVector.append(PointN);
        PointN.setX(-40);
        PointN.setY(260);
        PointVector.append(PointN);





        // стрілки ліній  і лінії області
                    for (int i=0;i<5;i++){
                     arrowAngle = atan2(PointVector[i].y() - PointVector[i+1].y(),PointVector[i].x()-PointVector[i+1].x() );
                      transform.translate(PointVector[i+1].x(),PointVector[i+1].y()); // перемыщення в кынець ліній
                       pointEnum[i+1].addToGroup(painter->addPolygon(polygon,regionPen)); // додання до масиву
                       pointEnum[i+1].setTransform(transform);
                       pointEnum[i+1].setRotation(180/M_PI * arrowAngle);  // поворот на кут
                       group_1->addToGroup(&pointEnum[i+1]);  // додаання до групи відображення
                    transform.translate(-(PointVector[i+1].x()), -(PointVector[i+1].y()));

                // лінії
                    group_1->addToGroup(painter->addLine(PointVector[i].x(),PointVector[i].y(),PointVector[i+1].x(),PointVector[i+1].y(),regionPen));
           // точки в вершинах прямокутника
                    group_1->addToGroup(painter->addEllipse(PointVector[i+1].x()-2, PointVector[i+1].y()-2, 4, 4,QPen(), QBrush(Qt::SolidPattern)));
            // піпис точок
                    textPathD.addText(PointVector[i].x() ,PointVector[i].y() -8, timesFont,tr("A %1").arg(i+1));
                    }
// початкова точка маневру
 group_1->addToGroup(painter->addEllipse(PointVector[6].x()-rad, PointVector[6].y()-rad, rad*2.0, rad*2.0,QPen(), QBrush(Qt::SolidPattern)));

                    for (int i=6;i<11;i++){
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
                    textPathD.addText(PointVector[i].x()+5 ,PointVector[i].y() -10, timesFont,tr("ХТТ %1").arg(i-5));
                    }
                    // підпис кінцевої точки маневру
                    textPathD.addText(PointVector[11].x()+5 ,PointVector[11].y() -10, timesFont,tr("ХТТ кін"));
        // стрілка  ліній підпису
        arrowAngle=90;
        for (int i=12;i<16;i++){

                  transform.translate(PointVector[i].x(),PointVector[i].y()); // перемыщення в кынець ліній
                   pointEnum[i+1].addToGroup(painter->addPolygon(polygon,pen2)); // додання до масиву
                   pointEnum[i+1].setTransform(transform);
                   pointEnum[i+1].setRotation( arrowAngle);  // поворот на кут
                   group_1->addToGroup(&pointEnum[i+1]);  // додаання до групи відображення
                transform.translate(-(PointVector[i].x()), -(PointVector[i].y()));
                arrowAngle +=180; // кут стрілок
                if (i == 13){
                arrowAngle = 180;
                }

        }

        // малювання  боквих ліній D1
        // малюємо в кординатах 0 0
        Line_D1.moveTo(170, 20);
        Line_D1.lineTo(360,20);

        Line_D2.moveTo(330,130);
        Line_D2.lineTo(360,130);
        // малювання   лінії L
        Line_D.moveTo(350,20);
        Line_D.lineTo(350,130);
        textPathD.addText(360, 70, timesFont,tr("L"));

        //
        group_1->addToGroup(painter->addPath(Line_D));
        group_1->addToGroup(painter->addPath(Line_D1));
        group_1->addToGroup(painter->addPath(Line_D2));

            // малювання   лінії D 2

          Line_D1.moveTo(-40,240);
          Line_D1.lineTo(-40,270);


        // малювання   лінії D
          Line_D.moveTo(-40, 260);
          Line_D.lineTo(0, 260);
          textPathD.addText(-20 , 280, timesFont,tr("D"));

        //
        group_1->addToGroup(painter->addPath(Line_D));
        group_1->addToGroup(painter->addPath(Line_D1));

        group_1->addToGroup(painter->addPath(textPathD,TextPen));

        // тарнформація на меобхідні кординати
        transform2.translate( 70 , -230);
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




//точки Х центру кола
void Region_Dialog::changedCenterX(){

 centerX = ui->center_x_edit->text().toDouble();
  setXY= true;

}
//точки Y центру кола
void Region_Dialog::changedCenterY(){
 centerY = ui->center_y_edit->text().toDouble();
  setXY= true;

}

//
void Region_Dialog::changedDistance(){
 Distance = ui->proces_distance_edit->text().toInt();

}
// зчитування кута  з вікна
void Region_Dialog::changedAngle(){
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
// вивід точок
QVector<PointOfTrace> Region_Dialog::getResPoint(){
    QPainterPath newPath;
    QVector<PointOfTrace> resPoints;
    ResRegionPath = QPainterPath();// обнулення полінії
    ResRegionPath.moveTo(PointRerionVector[0].x(),PointRerionVector[0].y());
    for (int i=1;i<PointRerionVector.size();i++){
        //додаємо кожну лінію
      ResRegionPath.lineTo(PointRerionVector[i].x(),PointRerionVector[i].y());
    }
    // зєднуємоє останню лінію з першою точкою
    ResRegionPath.lineTo(PointRerionVector[0].x(),PointRerionVector[0].y());
    QTransform transform1;
    transform1.rotate(0);
    //трансформуємо в нову лінію
   newPath = transform1.map(ResRegionPath);

for(int i = 0; i <  newPath.elementCount(); i++)
{
    QPointF pt = newPath.elementAt(i); //отримуэмо точку лінії в і позиції
    pt = returnGeoCoord(startPoint, pt.x(),pt.y());
    PointOfTrace PointOfTracer(pt.x(),pt.y(),startPoint);
        resPoints.append(PointOfTracer);

 }
return resPoints;
}
//виввид результучого шляху
QVector<PointOfTrace> Region_Dialog::getResPath(){
    QVector<PointOfTrace> resPoints;
    QVector<QPointF> newPoint;
     QVector<QPointF> pointUp;
     QVector<QPointF> pointDown;
   QPointF Point1;
   QPointF PointMaxX;
  QPointF PointminX;
   QPointF PointXdown;
   QPointF PointXup;
   QPointF PointminY;
   QPointF PointXup2;
    int numberXup;
    int numberXdown;
   qreal tan1;
   qreal kordinadYr;
   qreal Xp =0;
   qreal Yp =0;
   int maxX = 0; // обмеження крайної точки
   int minX = 0;
    ResPath = QPainterPath();
    QPainterPath newPath;
// розрахунок середньої точки фігури точок
    for(int i=0;i<PointRerionVector.size();i++){

       Xp += PointRerionVector[i].x();
       Yp +=PointRerionVector[i].y();
    }

    Xp=Xp/PointRerionVector.size();
    Yp=Yp/PointRerionVector.size();

    // перміщуємо фігуру точок в центр
   // ResRegionPath.translate(-Xp,-Yp);
    // повертаємо на потрібний кут
    QTransform transform1;
    transform1.rotate(0);
    //трансформуємо в нову лінію
   newPath = transform1.map(ResRegionPath);
// ствроємо вектор з ковими координатмами точок
    for(int i = 0; i <  newPath.elementCount()-1; i++)
    {
        QPointF pt = newPath.elementAt(i); //отримуэмо точку лінії в і позиції
        newPoint.append(pt);
        if ( i==0)
        {
            PointMaxX = PointminX =pt;
        }
        else{
        // пошук максимлного Х
            if(PointMaxX.x()<pt.x()){
            PointMaxX     = pt;
               maxX = i;

            }
            if( PointminX.x()>pt.x()){
           PointminX     = pt;
               minX = i;
            }
        }


    }
    // якщо маршрут проти голинкової стліки - п\вовернути й ого і перслати масив.
    if (minX >maxX) {
          newPath.toReversed();
          newPoint.clear();
          for(int i = 0; i <  newPath.elementCount()-1; i++)
          {
              QPointF pt = newPath.elementAt(i); //отримуэмо точку лінії в і позиції
              newPoint.append(pt);
              if ( i==0)
              {
                  PointMaxX = PointminX =pt;
              }
              else{
              // пошук максимлного Х
                  if(PointMaxX.x()<pt.x()){
                  PointMaxX     = pt;
                     maxX = i;

                  }
                  if( PointminX.x()>pt.x()){
                 PointminX     = pt;
                     minX = i;
                  }
              }
        }
    }

   // заповнення масиву гижних точкок
    int j =maxX; // номер максимальної точки
bool first = true;
    // від точки з максмальною кординаттою Х  до кінця масиву
    for(int i =j;i < newPoint.size() ; i++)
    {
        PointminY=newPoint[j];  //  точка - максимальна
        int c =j+1;
        // при с = більше розміру масиву - обнуляємо , ізнову продивляємость весь масив
        if (c >= newPoint.size()) c =0;
        // цик пошуку точокв яких Х менше максималоного на необхідну величину.
        for (int k =c;k < newPoint.size(); k++)
        {
             if (k == minX ) break;
            if( first ==true){
                if( PointminY.x()- Distance/2<newPoint[k].x() ||PointminY.x()==newPoint[k].x())
                    {
                         PointminY=newPoint[k]; // якщо такі є то це нова масимальна точка

                         j= k;
                    }

            }
            else{
           if( PointminY.x() -Distance<newPoint[k].x() ||PointminY.x()==newPoint[k].x())
               {
                    PointminY=newPoint[k]; // якщо такі є то це нова масимальна точка

                    j= k;
               }
        }

          // c++;
           // якщо чикл дойшов до кінця і не перглято всі еленети - почати з почвтку масиву
           if (k ==newPoint.size() ) k  =0;
           //якщо доедоно до мінімальної точки - вихід з циклу

           }
        // додавання результату в масив
        pointDown.append(PointminY);
        j++;
        first = false;
            if(j ==newPoint.size() ) {
                j =0;

            }
            //якщо доедоно до мінімальної точки - вихід з циклу
           if (minX ==j)
           {
                PointminY=newPoint[j];
               pointDown.append(PointminY);
               break;
           }
            // якщо цикл дойшов до кінця і не перглято всі еленети - почати з почвтку масиву
      if (i ==newPoint.size() ) i =0;
      // якщо обійли всі еленти - вихід з циклу.

      if (i == minX-1 ) break;

        }
 // заповнення масиву верхіінх точок точкок
    first = true;
     j =maxX;
    for(int i =j;i < newPoint.size(); i++)
    {
        PointminY=newPoint[j];
        int c =j;

        if (c == -1) c =newPoint.size();
        for (int k =c-1;k > 0; k--)   {
             if (k == minX ) break;
            if( first ==true){
                if( PointminY.x()  -Distance /2<newPoint[k].x() ||PointminY.x()==newPoint[k].x())
                {
                     PointminY=newPoint[k];
                     j= k;
                }
            }
            else
            {
                if( PointminY.x() -Distance<newPoint[k].x()||PointminY.x()==newPoint[k].x())
                {
                     PointminY=newPoint[k];
                     j= k;
                }
            }


          // c++;
           if (k ==newPoint.size() ) k  =0;

           }
        pointUp.append(PointminY);
        j--;
        if (minX ==j)
           {
               PointminY=newPoint[j];
               pointUp.append(PointminY);
                break;
         }
            if(j == -1 ) {
                j = newPoint.size()-1;
                }
      if (i == newPoint.size()-1 ) i =0;
    //  if (i == minX )
    //  {
    //      PointminY=newPoint[j];
      //    pointUp.append(PointminY);
      //     break;
    //  }


        }


 PointXdown = PointMaxX;
 PointXdown.setX( PointXdown.x()-Distance/2);
 PointXdown.setY(pointDown[0].y());
 PointXup= PointMaxX;
 PointXup.setX( PointXup.x()-Distance/2);
 PointXup.setY(pointUp[0].y());
// PointXdown2 = pointDown[1];
// PointXup2= pointUp[1];
 numberXup =0;
 numberXdown =0;
  int distance1 = Distance;

 // визначення кута між двома вершинами

 // почткова точка - умовний верх
 tan1 = atan2( pointUp[numberXup].y()-pointUp[numberXup+1].y(),pointUp[numberXup].x()-pointUp[numberXup+1].x());
          tan1 = tan( tan1);
          // визначення коринат за кутом
          // якщо тангес =0 то кордината У стала
               if (tan1 ==0){
                   kordinadYr=PointXup.y();
               }
               else{
                  //kordinadYr = -tan1 * PointXup.x() +PointXup.y()  ;
                   kordinadYr = PointXup.y() + (-tan1 *Distance )  ;
               }
          ResPath.moveTo(PointXup.x(), kordinadYr -Distance);
          PointXup.setY(kordinadYr );
         // PointXup.setX( PointXup.x()-Distance);

    do {

              //  першаточка умовнго низу
              tan1 = atan2( pointDown[numberXdown].y()-pointDown[numberXdown+1].y(),pointDown[numberXdown].x()-pointDown[numberXdown+1].x());
               tan1 = tan( tan1);
                            if (tan1 ==0 || pointDown[numberXdown+1].y()-pointDown[numberXdown].y() ==0){
                                kordinadYr=PointXdown.y();
                            }
                            else{
                              // kordinadYr =- tan1 * PointXdown.x()+PointXdown.y()   ;
                                kordinadYr =tan1 * PointXdown.x();
                            }
                       ResPath.lineTo(PointXdown.x(), kordinadYr +distance1);
                if(PointXdown.x() ==pointDown[numberXdown+1].x()) break;
                 if(PointXup.x() ==pointUp[numberXup+1].x()) break;
                       PointXdown.setX( PointXdown.x()-distance1);
                        PointXup.setX( PointXup.x()-distance1);


                if(PointXdown.x() <pointDown[numberXdown+1].x())
                {
                    numberXdown++;
                    if (numberXdown ==pointDown.size()-1) break;
                     if (PointXdown.x() - Distance < pointDown[pointDown.size()-1].x()) distance1 = Distance/2;
                    PointXdown.setY(pointDown[numberXdown].y());
                }
                if(PointXup.x() <pointUp[numberXup+1].x())
                {
                    numberXup++;
                    if (numberXup ==pointUp.size()-1) break;
                     if (PointXup.x() -  Distance <pointUp[pointUp.size()-1].x()) distance1 = Distance/2;
                    PointXup.setY(pointUp[numberXup].y());
                }
                if(PointXdown.x() ==pointDown[numberXdown+1].x())  PointXdown.setY(pointDown[numberXdown+1].y());
                 if(PointXup.x() ==pointUp[numberXup+1].x()) PointXup.setY(pointUp[numberXup+1].y());
               // друга точка умовного низу
                tan1 = atan2( pointDown[numberXdown].y()-pointDown[numberXdown+1].y(),pointDown[numberXdown].x()-pointDown[numberXdown+1].x());
                 tan1 = tan( tan1);
                              if (tan1 ==0|| pointDown[numberXdown+1].y()-pointDown[numberXdown].y() ==0){
                                  kordinadYr=PointXdown.y();
                              }
                              else{
                                  kordinadYr =tan1 * PointXdown.x();;
                              }
                         ResPath.lineTo(PointXdown.x(), kordinadYr +distance1);
                         //  першаточка умовнго верху
                  tan1 = atan2( pointUp[numberXup+1].y()-pointUp[numberXup].y(),pointUp[numberXup+1].x()-pointUp[numberXup].x());
                           tan1 = tan( tan1);

                                if (tan1 ==0|| pointUp[numberXup+1].y()-pointUp[numberXup].y()==0){
                                    kordinadYr=PointXup.y();
                                }
                                else{
                                  // kordinadYr = -tan1 * PointXup.x() +PointXup.y();  ;
                                     kordinadYr = PointXup.y() + (-tan1 * Distance)  ;
                                }
                           ResPath.lineTo(PointXup.x(), kordinadYr -distance1);
                                PointXup.setY(kordinadYr );

                           if(PointXdown.x() ==pointDown[numberXdown+1].x()) break;
                            if(PointXup.x() ==pointUp[numberXup+1].x()) break;
                           PointXdown.setX( PointXdown.x()-distance1);
                           PointXup.setX( PointXup.x()-distance1);

                            if(PointXdown.x() <pointDown[numberXdown+1].x())
                            {
                                numberXdown++;
                                if (numberXdown ==pointDown.size()-1 ) break;
                                if (PointXdown.x() - Distance < pointDown[pointDown.size()-1].x()) distance1 = Distance/2;
                                PointXdown.setY(pointDown[numberXdown].y());
                            }
                            if(PointXup.x() <pointUp[numberXup+1].x())
                            {
                                numberXup++;
                                if (numberXup ==pointUp.size()-1) break;
                                 if (PointXup.x() -  Distance<pointUp[pointUp.size()-1].x()) distance1 = Distance/2;
                                PointXup.setY(pointUp[numberXup].y());
                            }

                            if(PointXdown.x() ==pointDown[numberXdown+1].x())  PointXdown.setY(pointDown[numberXdown+1].y());
                             if(PointXup.x() ==pointUp[numberXup+1].x()) PointXup.setY(pointUp[numberXup+1].y());
    // друга точка умовного верху
                          tan1 = atan2( pointUp[numberXup].y()-pointUp[numberXup+1].y(),pointUp[numberXup].x()-pointUp[numberXup+1].x());
                                   tan1 = tan( tan1);
                                           if (tan1 ==0 ||pointUp[numberXup+1].y()-pointUp[numberXup].y()==0){
                                             kordinadYr=PointXup.y();
                                           }
                                           else{
                                             // kordinadYr = -tan1 *PointXup.x() +PointXup.y()  ;
                                                kordinadYr = PointXup.y() + (-tan1 * Distance)  ;
                                           }
                                      ResPath.lineTo(PointXup.x(), kordinadYr -distance1);
                                      PointXup.setY(kordinadYr );

    }

          while (1!=2);
transform1.rotate(-Angle*2);
//ResPath = transform1.map(ResPath);
//ResPath.translate(Xp,Yp);

for(int i = 0; i <  ResPath.elementCount(); i++)
{
    QPointF pt = ResPath.elementAt(i); //отримуэмо точку лінії в і позиції
    pt = returnGeoCoord(startPoint, pt.x(),pt.y());
    PointOfTrace PointOfTracer(pt.x(),pt.y(),startPoint);
    resPoints.append(PointOfTracer);
 }
return resPoints;
}
//додавання точки області
void Region_Dialog::add_point(){
    if( Poinchanged == true ){

      QPointF PointR(ui->point_x_edit->text().toInt(),ui->point_y_edit->text().toInt()); // змвнн для задання очок
      // PointR.setX(ui->point_x_edit->text().toInt());
      // PointR.setY(ui->point_y_edit->text().toInt());
       bool  PoinChanged= true;
       for (int i =0; i<PointRerionVector.size();i++)
       {
           if(  PointR == PointRerionVector[i]) {
               PoinChanged = false;
               break;
           }
       }
       if(PoinChanged == true){
      PointRerionVector.append(PointR);
    // выдображення в таблицы

      // знаходимо ыіндекс за розміром масиву
      int PRvectorSize = PointRerionVector.size() -1 ;
      // задаємо кількість стопчиків
      ui->tableWidget->setRowCount(PRvectorSize );
      ui->tableWidget->insertRow(PRvectorSize);

      //вивівд занчень,в строковому форматі
      ui->tableWidget->setItem(PRvectorSize  ,0, new QTableWidgetItem(tr("%1").arg(PointRerionVector[PRvectorSize].x())));
      ui->tableWidget->setItem(PRvectorSize,1, new QTableWidgetItem(tr("%1").arg(PointRerionVector[PRvectorSize].y())));
      // мастабування ширини
       ui->tableWidget->resizeColumnsToContents();
       // зробии\ти активною кнопку видалення
       ui->delete_Point_Button->setEnabled(true);
       ui->point_x_edit->setText("");
       }
       else
          QMessageBox::critical(this,"помилка","така точка вже існує");
       ui->point_x_edit->setText("");
       ui->point_y_edit->setText("");
       Poinchanged = false;
    }
    else {
       QMessageBox::critical(this,"помилка","не введо координати точки");
    }
}
//виделення точки з таблиці

void Region_Dialog::removePointOfTraceable() {
// множина точокж
    QSet<int> selectedRows;
   // шукаємо виділену точку
       foreach (QModelIndex index, ui->tableWidget->selectionModel()->selectedRows()){
           selectedRows.insert(index.row());
       }
       // якщо немає виділеної точки - видаляємо останню
       if (selectedRows.capacity() ==0 )
       {
            ui->tableWidget->removeRow(PointRerionVector.size() -1);
            // видалення з масиву точок
          PointRerionVector.remove(PointRerionVector.size() -1);
       }
       else
       {
           foreach(int row, selectedRows){
               ui->tableWidget->removeRow(row);
                PointRerionVector.remove(row);
           }
       }

       //якщо точок немає - зробити видалення недоступним
       if(PointRerionVector.size() == 0){
            ui->delete_Point_Button->setEnabled(false);
    }

}

void Region_Dialog::acceptR()
{
    if( PointRerionVector.size()>=3){
         emit accepted();
    }
    else {
       QMessageBox::critical(this,"помилка","ведено недостаньо точок");
    }

}

void Region_Dialog::coordChange()
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

Region_Dialog::~Region_Dialog()
{
    delete ui;
}
bool Region_Dialog::getSetXY(){
      return  setXY;
}
