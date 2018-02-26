#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circledialog.h"
#include <QFileDialog>
#include<QPushButton>
#include<QMessageBox>
#include <qmath.h>
#include "line_dialog.h"
#include "box_line_dialog.h"
#include "baterfly_dialog.h"
#include "endless_dialog.h"
#include "snake_dialog.h"
#include "region_dialog.h"
#include "manevr_item.h"
#include "stratecoord.h"
#include "march_dialog.h"
#include "point_dialog.h"
#include <QDomDocument>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //всвтака коду управлыння лыный

    //завдання паремтрів відображення
    //вчтановлення точки початку маршруту
     endManevrPoint.setX(0);
      endManevrPoint.setY(0);
      startPointSetet = false;
      newPolyline = true;
   // ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    connect(ui->action_Cirle,SIGNAL(triggered(bool)),this,SLOT(Action_circle()));
    connect(ui->action_Line,SIGNAL(triggered(bool)),this,SLOT(Action_Line()));
    connect(ui->action_open_File,SIGNAL(triggered(bool)),this,SLOT(Action_Open_File()));
    connect(ui->action_Box_line,SIGNAL(triggered(bool)),this,SLOT(Action_Box_Line()));
    connect(ui->action_Baterfly,SIGNAL(triggered(bool)),this,SLOT(Action_Baterfly_Dialog()));
    connect(ui->action_Endless,SIGNAL(triggered(bool)),this,SLOT(Action_Endless()));
    connect(ui->action_Snake,SIGNAL(triggered(bool)),this,SLOT(Action_Snake_Dialog()));
    connect(ui->action_Region_line,SIGNAL(triggered(bool)),this,SLOT(Action_Region_Dialog()));
    connect(ui->action_Route,&QAction::triggered,this,&MainWindow::Action_March);
    connect(ui->action_Add_startPoint,&QAction::triggered,this,&MainWindow::Action_addStartPoint);
    connect(ui->start_Point_buton,&QPushButton::pressed,this,&MainWindow::addStartPoint);
    connect(ui->action_save_file,&QAction::triggered,this,&MainWindow::saveKml);
   // connect(ui->zoom_Slider,SIGNAL(valueChanged(int)),this,SLOT(SetZoom()));
    // ынтерфейс малювання
    //connect(ui->line_buton, &QToolButton::clicked, [=](){workplaceScene->setCurrentAction(VEWorkplace::LineType);});
    connect(ui->select_buton, &QToolButton::clicked, [this]{disconnect(mc, SIGNAL(mouseEventCoordinate(const QMouseEvent*, const QPointF)),this, SLOT(writeStartPoint(const QMouseEvent*, const QPointF)));});
    // події при виборі графічних інтерфейсіфв

//робимо елементи неактивнми
    ui->action_Baterfly->setEnabled(false);
    ui->action_Box_line->setEnabled(false);
    ui->action_Cirle->setEnabled(false);
    ui->action_Endless->setEnabled(false);
    ui->action_Line->setEnabled(false);
    ui->action_Region_line->setEnabled(false);
    ui->action_Route->setEnabled(false);
    ui->action_save_file->setEnabled(false);
    ui->action_Snake->setEnabled(false);

//qmap begin
// tзадаємо розмір полотна карти
QSize size = QSize(880,640);
// створюємо обєкт відображення
mc = new MapControl(size);
mc->enablePersistentCache();
// полтика розміру
mc->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

ui->horizontalLayout->addWidget(mc);
// створюємо адаптер карти
MapAdapter* mapadapter = new OSMMapAdapter();
// створюємо шар відображення елементів
layer1 = new MapLayer("Custom Layer", mapadapter);

mc->addLayer(layer1);

// задаємо початкову точку відображення

mc->setView(QPointF( 30.421598, 50.437246));
mc->setZoom(11);
// малюємо кнопис зміни масштабу.
addZoomButtons();
//qmap end


}

MainWindow::~MainWindow()
{
    delete ui;

    delete mc;
}

// створення діалогу з вибором кола
void MainWindow::Action_circle(){
    mcirleDialog =new CircleDialog(startPointF, this);
    connect(mcirleDialog,SIGNAL(accepted()),this,SLOT(set_Circle()));
  mcirleDialog->show();
}
//
void MainWindow::Action_Line(){
     mlineDialog =new Line_Dialog(startPointF, this);
      connect(mlineDialog,&Line_Dialog::accepted,this,&MainWindow::set_Line);
  mlineDialog->show();
}

// відкриття файлу картинки
void MainWindow::Action_Open_File(){
    FileName1 = QFileDialog::getOpenFileName(this,"Відкрити файл KML",QDir::homePath(),"KML files(*.kml);;all files (*.*)");
    //якщо вибрано файлу - вихід
    if (FileName1.isEmpty()){
        return;
    }

}
//
void MainWindow::Action_March(){
    mMarch_Dialog =new march_Dialog(manevrsList,routeName ,this);
     connect(mMarch_Dialog,&march_Dialog::accepted,this,&MainWindow::manevr_Edit,Qt::UniqueConnection);

 mMarch_Dialog->show();

}
//відктия файлу Kml
void MainWindow::openKml()
{

    QString filename = QFileDialog::getOpenFileName(this, " open klm");
    if (filename=="")
        return;

    QDomDocument doc;       // объект документа
    QFile file(filename);   // Открываем наш SVG-файл
    // Если он не открылся или не удалось передать содержимое в QDocDocument
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return ;    // то возвратим список, но пустой

    // Ищем в документе все объекты с тегом g
   /* QDomNodeList gList = doc.elementsByTagName("g");
    // Начинаем их перебирать
    for (int i = 0; i < gList.size(); i++) {
        QDomNode gNode = gList.item(i);     // Выделяем из списка ноду
        QDomElement rectangle = gNode.firstChildElement("rect");    // И ищем в ней элемент c тегом rect
        // Если полученный элементы не нулевой, то
        if (rectangle.isNull()){
            continue;
        } else {
            // начинаем формировать прямоугольник
            QGraphicsRectItem *rect = new QGraphicsRectItem();
            // Этот флаг делает объект перемещаемым, потребуется для проверки
            rect->setFlag(QGraphicsItem::ItemIsMovable);
            // Забираем размеры из тега rect
            QDomElement gElement = gNode.toElement();
            rect->setRect(rectangle.attribute("x").toInt(),
                          rectangle.attribute("y").toInt(),
                          rectangle.attribute("width").toInt(),
                          rectangle.attribute("height").toInt());

             Забираем из элемента ноды gNode параметры цветов
             * да да да... именно из gNode, а не из rectangle. Эти параметры храняться в теге g
             * */
    /*        QColor fillColor(gElement.attribute("fill", "#ffffff"));    // цвет заливки
            fillColor.setAlphaF(gElement.attribute("fill-opacity","0").toFloat());
            rect->setBrush(QBrush(fillColor));

            // а также цвет и толщина абриса
            QColor strokeColor(gElement.attribute("stroke", "#000000"));
            strokeColor.setAlphaF(gElement.attribute("stroke-opacity").toFloat());

            rect->setPen(QPen(strokeColor,gElement.attribute("stroke-width", "0").toInt()));
            rectList.append(rect);  // добавляем прямоугольник в список
        }
    }
    */
    file.close();



    //


}
//зберігання в  файлу Kml
void MainWindow::saveKml()
{
    QString filename = QFileDialog::getSaveFileName(this,"Зберегрегти файл",QDir::homePath(),"kml files(*.kml);;all files (*.*)");
    if (filename=="")
        return;
    QDomDocument doc;       // объект документа
// вікдрпиваємо файл шаблон
    QFile file("route1.kml");    
     // Если он не открылся или не удалось передать содержимое в QDocDocument
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return ;    //  виходимо з методу ( треба пермістити файл в ресурси
// шукаємо елент координат
     QDomNodeList gList = doc.elementsByTagName("coordinates");
     QDomNode gNode = gList.item(0);
   // робимо йго активним елементом
     QDomElement domElement = gNode.toElement();
     QString resultString;
     //пертворєюмо кординати маршруту в строки
     for (int i=0;i<routePoints.size();i++) {
         Point *routPoint =  routePoints.at(i);
     resultString = QString("%1 %2,%3,0").arg(resultString).arg(routPoint->longitude()).arg(routPoint->latitude());
     }
             //длодаємо до активного елента текст
     QDomText domText = doc.createTextNode(resultString);
     domElement.appendChild(domText);
     // робимо запис у новий файл
            QFile fileo(filename);
                if(fileo.open(QIODevice::WriteOnly)) {
                    QTextStream(&fileo) << doc.toString();
                    fileo.close();
                }
                file.close();
}
// додавання фтратової точки з меню
void MainWindow::Action_addStartPoint(){
    QPointF str(0,0);
    PointOfTrace resivePoint(0,0,str);
    mPoint_Dialog =new Point_Dialog(resivePoint,this,true);
     connect(mPoint_Dialog,&Point_Dialog::accepted,this,&MainWindow::set_startPoint);
    //connect(mMarch_Dialog,SIGNAL(a))
   // mMarch_Dialog->setManevrList(manevrsList);
 mPoint_Dialog->show();
}


// отримання параметрві кола
void MainWindow::set_Circle(){
    QVector<PointOfTrace> Line_vector;
        Line_vector =  mcirleDialog->getResPath();
      paintingManevr(Line_vector,mcirleDialog->getSetXY(),false,1);

delete mcirleDialog;
}
// додавання лінії
void MainWindow::set_Line(){
    QVector<PointOfTrace> Line_vector;
    Line_vector = mlineDialog->getResPoint();
    paintingManevr(Line_vector,mlineDialog->getSetXY(),false,2);

delete mlineDialog;

    }
void MainWindow::set_Box_Line(){

    QVector<PointOfTrace> Line_vector;
       Line_vector =  mBox_Line_Dialog->getResPath();
  paintingManevr(Line_vector,mBox_Line_Dialog->getSetXY(),false,3);

  delete mBox_Line_Dialog;

}
//
void MainWindow::set_Baterfly(){

QVector<PointOfTrace> Line_vector;

   Line_vector =  mBaterfly_Dialog->getResPath();
 paintingManevr(Line_vector,mBaterfly_Dialog->getSetXY(),false,4);

 delete mBaterfly_Dialog;
}
//
void MainWindow::set_Endless(){

    QVector<PointOfTrace> Line_vector;
   Line_vector =  mEndless_Dialog->getResPath();
   paintingManevr(Line_vector,mEndless_Dialog->getSetXY(),false,5);
   delete mEndless_Dialog;
}
void MainWindow::set_Snake(){
    QVector<PointOfTrace> Line_vector;
   Line_vector =  mSnake_Dialog->getResPath();
  paintingManevr(Line_vector,mSnake_Dialog->getSetXY(),false,6);
  delete mSnake_Dialog;
}
void MainWindow::set_Region(){

    QVector<PointOfTrace> Line_vector;
   Line_vector =  mRegion_Dialog->getResPoint();
 //paintingManevr(Line_vector,false,false);
   Line_vector.clear();

  Line_vector =  mRegion_Dialog->getResPath();
 paintingManevr(Line_vector,mRegion_Dialog->getSetXY(),true,7);
 //delete mRegion_Dialog;

}


// створення діалогу з вибором кола
void MainWindow::Action_Box_Line(){
   mBox_Line_Dialog = new Box_Line_Dialog (startPointF, this);
    connect(mBox_Line_Dialog,&Box_Line_Dialog::accepted,this,&MainWindow::set_Box_Line);
  mBox_Line_Dialog->show();
}

// створення діалогу з вибором кола
void MainWindow::Action_Baterfly_Dialog(){
    mBaterfly_Dialog = new Baterfly_Dialog (startPointF, this);
    connect(mBaterfly_Dialog,&Baterfly_Dialog::accepted,this,&MainWindow::set_Baterfly);
  mBaterfly_Dialog->show();
}
//
void MainWindow::Action_Endless(){
     mEndless_Dialog = new Endless_Dialog (startPointF, this);
    connect( mEndless_Dialog,&Endless_Dialog::accepted,this,&MainWindow::set_Endless);
   mEndless_Dialog->show();
}
//
void MainWindow::Action_Snake_Dialog(){
    mSnake_Dialog = new Snake_Dialog (startPointF, this);
    connect(mSnake_Dialog,&Snake_Dialog::accepted,this,&MainWindow::set_Snake);
   mSnake_Dialog->show();
}

//
void MainWindow::Action_Region_Dialog(){
    mRegion_Dialog = new Region_Dialog (startPointF, this);
    connect(mRegion_Dialog,&Region_Dialog::accepted,this,&MainWindow::set_Region);
    mRegion_Dialog->show();
}


//
//void MainWindow::checkSelection()
//{
  /*  checkActionStates();
    switch (workplaceScene->selectedItems().length()) {
    case 0:
       // ui->rectangleSettings->deselect();
        ui->polylineSettings->deselect();
        checkActionStates();
        //ui->toolsWidget->setEnabled(true);
        break;
    case 1:
        switch (workplaceScene->selectedItems().at(0)->type()) {
        case QGraphicsRectItem::Type:{
            //ui->rectangleSettings->setVisible(true);
            ui->polylineSettings->deselect();
            ui->polylineSettings->setVisible(false);
            break;
        }
        default: {
            //ui->rectangleSettings->deselect();
            //ui->rectangleSettings->setVisible(false);
            ui->polylineSettings->setVisible(true);
            break;
        }
        }
        checkActionStates();
   //     ui->toolsWidget->setEnabled(true);
        break;
    default:
      //  ui->rectangleSettings->deselect();
        ui->polylineSettings->deselect();
      //  ui->toolsWidget->setEnabled(false);
        break;
    }
    */
 //}

//void MainWindow::checkActionStates()
//{
   /* ui->workplaceView->setDragMode(QGraphicsView::NoDrag);
  //  ui->rectangleSettings->setVisible(false);
    ui->polylineSettings->setVisible(false);
    switch (workplaceScene->currentAction()){
    case VEWorkplace::LineType:
        ui->polylineSettings->setVisible(true);
        break;
    //case VEWorkplace::RectangleType:
   //     ui->rectangleSettings->setVisible(true);
      //  break;
    case VEWorkplace::SelectionType:
        break;
    default:
       // ui->workplaceView->setDragMode(QGraphicsView::ScrollHandDrag);
        break;
    }
    */
//}

//void MainWindow::selectItem(QGraphicsItem *item)
//{
   /* switch (item->type()) {
    case QGraphicsRectItem::Type: {
    //    VERectangle *rect = qgraphicsitem_cast<VERectangle *>(item);
    //    ui->rectangleSettings->loadRectangle(rect);
        break;
    }
    case QGraphicsPathItem::Type: {
        VEPolyline *polyline = qgraphicsitem_cast<VEPolyline *>(item);
        ui->polylineSettings->loadPolyline(polyline);
        break;
    }
    default:
        break;
    }
    */
//}

//void MainWindow::selectNewItem(QGraphicsItem *item)
//{
  /*  switch (item->type()) {

    case QGraphicsPathItem::Type: {
        VEPolyline *polyline = qgraphicsitem_cast<VEPolyline *>(item);
        ui->polylineSettings->newPolyline(polyline);
        break;
    }
    default:
        break;
    }
    */
//}

//

void MainWindow::addZoomButtons()
{
    // create buttons as controls for zoom
    QPushButton* zoomin = new QPushButton("+");
    QPushButton* zoomout = new QPushButton("-");
    zoomin->setMaximumWidth(50);
    zoomout->setMaximumWidth(50);

    connect(zoomin, SIGNAL(clicked(bool)),
              mc, SLOT(zoomIn()));
    connect(zoomout, SIGNAL(clicked(bool)),
              mc, SLOT(zoomOut()));

    // add zoom buttons to the layout of the MapControl
    QVBoxLayout* innerlayout = new QVBoxLayout;
    innerlayout->addWidget(zoomin);
    innerlayout->addWidget(zoomout);
    mc->setLayout(innerlayout);
}

void MainWindow::geometryClicked(Geometry* geom, QPoint)
{
  /*  qDebug() << "parent: " << geom->parentGeometry();
    qDebug() << "Element clicked: " << geom->name();
    if (geom->hasClickedPoints())
    {
        QList<Geometry*> pp = geom->clickedPoints();
          QMessageBox::information(this, geom->name(),QString(geom->objectName()));

      //  qDebug() << "number of child elements: " << pp.size();
         qDebug() << geom->name();

         for (int i=0;i<manevrsList.size();i++){
         //foreach ( manevr_Item *item, manevrsList()) {
               manevr_Item manevrPaint = manevrsList.at(i);
             if(geom->name() == manevrPaint.ManevrName()) {
                 selectedManevr= i;
                  qDebug() << i;
                 break;
             }
         }
                   //      removeItem(item);
                  //       delete item;
         // qDebug() << geom->metaObject();
      //    qDebug() << geom->toString();
//
       //  geom->deleteLater();
      //  for (int i=0; i<pp.size(); ++i)
        //{
           // QMessageBox::information(this, geom->name(), pp.at(i)->name());
            //QMessageBox::information(this, geom->name(), "figure");
       // }
    }
    else if (geom->GeometryType == "Point")
    {
        QMessageBox::information(this, geom->name(), "just a point");
    }
    */
}



void MainWindow::resizeEvent(QResizeEvent *qEvent)
{
    Q_UNUSED( qEvent );
    if (mc)
    {
        mc->resize(size());
    }

}

//
void MainWindow::set_startPoint(){
    QPointF resivePoint;
    QVector<PointOfTrace> startPointV;
    resivePoint =mPoint_Dialog->return_editPoint();


    startPoint= new ImagePoint(resivePoint.x(), resivePoint.y(), "images/Location_Tag.png", "start", Point::BottomMiddle);
     startPointF = resivePoint;
     QPen* linepen = new QPen(QColor(0, 0, 255, 100));
linepen->setWidth(5);
        // Points with image
    routePoints.append(startPoint);
    //points.append(startPoint2);
    LineString* ls = new LineString(routePoints, "Старт", linepen);
    // Add the LineString to the layer
    layer1->addGeometry(ls);
    mPoint_Dialog->deleteLater();
    PointOfTrace startPointOT= PointOfTrace(resivePoint.x(),resivePoint.y(),startPointF);
 startPointV.append(startPointOT);
    manevr_Item manevr1( startPointV,"Старт",startPointF);
     manevrsList.append(manevr1);

    ui->action_Baterfly->setEnabled(true);
    ui->action_Box_line->setEnabled(true);
    ui->action_Cirle->setEnabled(true);
    ui->action_Endless->setEnabled(true);
    ui->action_Line->setEnabled(true);
    ui->action_Region_line->setEnabled(true);
    ui->action_Route->setEnabled(true);
    ui->action_save_file->setEnabled(true);
    ui->action_Snake->setEnabled(true);
    routeName ="маршрут 1 ";

}
// створення точки старту
void MainWindow::writeStartPoint(const QMouseEvent* eventClic, const QPointF coord)
{
    QVector<PointOfTrace> manevrsPoints;
     PointOfTrace manevrPoint;
     QVector<PointOfTrace> startPointV;
    if (eventClic->type()==QMouseEvent::MouseButtonDblClick){
        QPen* linepen = new QPen(QColor(0, 0, 255, 100));
   linepen->setWidth(5);
           // Points with image

        if (startPointSetet== false){
    QMessageBox::information (this,"Старт",QString("Встановлено точку старту \n з координатами \nширота %1  довгота %2 ")
                             .arg(coord.x())
                             .arg(coord.y()));

    QPixmap* pixm = new QPixmap(100,200);
     pixm->load("images/Location_Tag.png");
    //pixm->fill(Qt::transparent);
    QPainter pain(pixm);
    pain.setFont(QFont("Helvetiva", 8));
    //QRectF *textRect = new QRectF(10,20,20,30);
    pain.setPen(Qt::red);

    pain.drawText(QPoint(4,25), "Старт");
    pain.end();
     startPoint= new ImagePoint(coord.x(), coord.y(), *pixm, "Старт", Point::BottomMiddle);
     startPointF =  QPointF(coord.x(), coord.y());
     startPointSetet = true;
     PointOfTrace startPointOT= PointOfTrace(coord.x(),coord.y(),startPointF);
  startPointV.append(startPointOT);
     manevr_Item manevr1( startPointV,"Старт",startPointF);
      manevrsList.append(manevr1);
   routePoints.append(startPoint);
   ui->action_Baterfly->setEnabled(true);
   ui->action_Box_line->setEnabled(true);
   ui->action_Cirle->setEnabled(true);
   ui->action_Endless->setEnabled(true);
   ui->action_Line->setEnabled(true);
   ui->action_Region_line->setEnabled(true);
   ui->action_Route->setEnabled(true);
   ui->action_save_file->setEnabled(true);
   ui->action_Snake->setEnabled(true);
   routeName ="маршрут 1 ";
        }
        else
        {

         manevrPoint = PointOfTrace(coord.x(),coord.y(),startPointF);
         if(newPolyline == true) {
                 newPolyline = false;
                  QString manevrName;
                 manevrsPoints.append(manevrPoint);
                 // задаємо назву
                 manevrName=QString("Полі-лінія%1").arg(manevrNameN[8]);
                 manevrNameN[8]= manevrNameN[8] + 1;
          // створюємо манев

           manevr_Item manevr1(manevrsPoints,manevrName,startPointF);
             manevrsList.append(manevr1);
         }
         else {
              manevr_Item manevr1 = manevrsList.at(manevrsList.size()-1);
              manevrsPoints = manevr1.getPointsNumber();
              manevrsPoints.append(manevrPoint);
              manevr1.setPointsNumber(manevrsPoints);
              manevrsList.removeLast();
               manevrsList.append(manevr1);
         }

      }
        updatePainingManevr();

   //  LineString* ls = new LineString(routePoints, "Старт", linepen);
     // Add the LineString to the layer
    // layer1->addGeometry(ls);

     connect(layer1, SIGNAL(geometryClicked(Geometry*, QPoint)),
               this, SLOT(geometryClicked(Geometry*, QPoint)));
   // disconnect(mc, SIGNAL(mouseEventCoordinate(const QMouseEvent*, const QPointF)),
     //         this, SLOT(writeStartPoint(const QMouseEvent*, const QPointF)));
    endManevrPoint.setX(coord.x());
     endManevrPoint.setY(coord.y());
    }
}

//режим додавання точки старту
void MainWindow::addStartPoint(){
    //
    connect(mc, SIGNAL(mouseEventCoordinate(const QMouseEvent* , const QPointF)),
            this, SLOT(writeStartPoint(const QMouseEvent* , const QPointF)));

}
// додавання маневра до відображення

void MainWindow::paintingManevr(QVector<PointOfTrace> listPoints, bool conectionline, bool survey, int manevrType){
    QPointF point1; // точка для трасформації кодинат
   //QVector<PointOfTrace> manevrsPoints;  //точки маневру
// для кожної точки маршруту
    //int i;
    //if(conectionline==true) i=0;
     //       else
      //      i=1;
 //for(int i=0;i< listPoints.size();i++)
// {
     // за потреби перретворюємо в георгрвфічні корлинати
  //   point1=  trans1.returnGeoCoord(point2, listPoints[i].x(), listPoints[i].y());

  //ImagePoint* manevrPoint= new ImagePoint( point1.x(),  point1.y(), *pixm, QString("start%1").arg(i), corection);
   // routePoints.append(manevrPoint);
    // додоємо загальний номер точки до маневру
   // manevrsPoints.append(point1);
 //}
// створюємо маневр
 QString manevrName;
 switch (manevrType) {
 case 1:
     manevrName=QString("Круг%1").arg(manevrNameN[1]);
     manevrNameN[1]= manevrNameN[1] + 1;
     break;
 case 2:
     manevrName=QString("Відрізок%1").arg(manevrNameN[2]);
     manevrNameN[2]= manevrNameN[2] + 1;
     break;
 case 3:
     manevrName=QString("Прямокутник%1").arg(manevrNameN[3]);
     manevrNameN[3]= manevrNameN[3] + 1;
     break;
 case 4:
     manevrName=QString("Метелик%1").arg(manevrNameN[4]);
     manevrNameN[4]= manevrNameN[1] + 1;
     break;
 case 5:
     manevrName=QString("Вісімка%1").arg(manevrNameN[5]);
     manevrNameN[5]= manevrNameN[5] + 1;
     break;
 case 6:
     manevrName=QString("Змійка%1").arg(manevrNameN[6]);
     manevrNameN[6]= manevrNameN[6] + 1;
     break;
 case 7:
     manevrName=QString("Область%1").arg(manevrNameN[7]);
     manevrNameN[7]= manevrNameN[7] + 1;
     break;
 case 8:

     break;

 default:
     break;
 }
 manevr_Item manevr1(listPoints,manevrName,startPointF);
 //manevrsList.append(manevr_Item(manevrsPoints,QString("Manevr_%1").arg(manevrsList.size())));
 if( survey == true){ // якшо є область встаноавлюємо її точки

     QVector<PointOfTrace> Line_vector;
    Line_vector =  mRegion_Dialog->getResPoint();
        manevr1.setSurveyNumber(Line_vector);
  delete mRegion_Dialog;
 }
  manevrsList.append(manevr1);
// задаэмо кынцеву точку маневру
           endManevrPoint.setX(point1.x());
            endManevrPoint.setY(point1.y());
     // робимо выдображення
       updatePainingManevr();
       newPolyline = true;
}
// головний вивід маршруту на карту
/*void MainWindow::mainPaitingManevr(){
     // задаємо стиль малювання
    QPen* linepen = new QPen(QColor(0, 0, 255, 100));
    linepen->setWidth(5);
    //QPen* surveyPen = new QPen(QColor(0, 255, 0, 100));
   // surveyPen->setWidth(4);
   // surveyPen->setDashPattern(Qt::DashDotLine);
     // створюємо маневр ма шарі картиж
    LineString* ls = new LineString(routePoints, "Старт", linepen);
    //    LineString* surveyString = new LineString( surveyPoints, "", surveyPen);
    // додаємо лінії на шар малювання
    layer1->addGeometry(ls);
     //layer1->addGeometry(surveyString);
}
*/
// зміна маршрктк на карті
void MainWindow::updatePainingManevr(){
    QPointF point1; // точка для трасформації кодинат 
    QPointF point2 ;// кординати точкистарту
    QPointF lastPoint =point2;
    QPointF arrow2point =point2;
    QList<Point*> newRoute;

    // вістановлюємо точку старту
    layer1->clearGeometries();
   QPen* linepen = new QPen(QColor(0, 0, 255, 100));
    linepen->setWidth(5);

  /*  QPixmap* pixm = new QPixmap(100,200);
     pixm->load("images/Location_Tag.png");
    //pixm->fill(Qt::transparent);
    QPainter pain(pixm);
    pain.setFont(QFont("Helvetiva", 8));
    //QRectF *textRect = new QRectF(10,20,20,30);
    pain.setPen(Qt::red);

    pain.drawText(QPoint(4,25), "Старт");
    pain.end();

  //  QVector<PointOfTrace> pointNumber;
    ImagePoint* manevrPoint= new ImagePoint( point2.x(),  point2.y(), *pixm, QString("start"), Point::BottomMiddle);

    //newRoute.append(manevrPoint);
*/
   int startPointE =0;
    //layer1->addGeometry(manevrPoint);
    // фігура стрілки
    QPixmap* arrowPix = new QPixmap(100,200);
    arrowPix->load("images/leftarrowsign.png");
    arrowPix->detach();

        // для кожного маневру
         for(int i=0;i< manevrsList.size();i++)
         {

              QVector<PointOfTrace> pointNumber;
              manevr_Item manevrPaint = manevrsList.at(i);
              QList<Point*> manevrRoute;
              // отримуємлон номери точток маневру
             pointNumber =  manevrPaint.getPointsNumber();
             // для кожної точки маневру
             for (int k=0 ;k<manevrPaint.getRepeatNumber();k++){
                 for( int j=0; j<pointNumber.size();j++)
                 {

                     point1 = pointNumber[j].getGeoPoint();
                     if (i==0){
                         QPixmap* pixm = new QPixmap(100,200);
                          pixm->load("images/Location_Tag.png");
                         //pixm->fill(Qt::transparent);
                         QPainter pain(pixm);
                         pain.setFont(QFont("Helvetiva", 8));
                         //QRectF *textRect = new QRectF(10,20,20,30);
                         pain.setPen(Qt::red);

                         pain.drawText(QPoint(4,25), "Старт");
                         pain.end();

                       //  QVector<PointOfTrace> pointNumber;
                         ImagePoint* manevrPoint= new ImagePoint( point1.x(),  point1.y(), *pixm, QString("start"), Point::BottomMiddle);

                         //newRoute.append(manevrPoint);
                         newRoute.append(manevrPoint);
                         manevrRoute.append(manevrPoint);
                          point2 =point1;// кординати точкистарту
                        lastPoint =point2;
                          arrow2point =point2;
                          startPointF =point1;
                         continue;
                     }
                     if ( j==0 && point1.x()!=lastPoint.x() &&point1.y() !=lastPoint.y()){
                        // Point conectPoint( point1.x(),  point1.y(), "1",Point::BottomMiddle);
                         ImagePoint* manevrPoint= new ImagePoint( lastPoint.x(),  lastPoint.y(), "1", "1", Point::BottomMiddle);
                       newRoute.append(manevrPoint);
                          manevrRoute.append(manevrPoint);
                          startPointE++;

                     }

                    if(point1 == point2 && newRoute.size()==1){

                             ImagePoint* manevrPoint= new ImagePoint( point1.x(),  point1.y(), "images/Location_Tag.png", QString("point%1").arg(i), Point::BottomMiddle);
                        newRoute.append(manevrPoint);
                        manevrRoute.append(manevrPoint);
                        startPointE++;
                                }

                            else
                            {

                                //  малювання еомера точкиномер точки
                                QPixmap* pixm = new QPixmap(100,200);
                                 pixm->load("images/Location_Tag.png");
                                //pixm->fill(Qt::transparent);
                                QPainter pain(pixm);
                                pain.setFont(QFont("Helvetiva", 16));
                                //QRectF *textRect = new QRectF(10,20,20,30);
                                pain.setPen(Qt::red);
                                if(newRoute.size()<10+startPointE)
                                pain.drawText(QPoint(15,30), QString().setNum(newRoute.size()-startPointE));
                                else
                                 pain.drawText(QPoint(5,30), QString().setNum(newRoute.size()-startPointE));
                                pain.end();
                                //
                                   ImagePoint* manevrPoint= new ImagePoint( point1.x(),  point1.y(), *pixm, QString("point%1").arg(i), Point::BottomMiddle);
                                   newRoute.append(manevrPoint);
                                    manevrRoute.append(manevrPoint);


                                   //малювання стілки
                                   // кут стірілки
                                   qreal arrowAngle= atan2( arrow2point.y()-point1.y(),arrow2point.x() - point1.x());
                                                    //arrowAngle = tan( arrowAngle);
                                   QTransform transform1;
                                   // трасформація  у градуси
                                   arrowAngle=-arrowAngle*180/M_PI;
                                   // черезз особливічсь алгоритму необхідно тсрасформувати додатково на 8 градусві, цілі кцти( 0, 90,180 трасформувати не потрібно
                                   if(arrowAngle<0 && arrowAngle>-90) arrowAngle =arrowAngle-8;
                                   if(arrowAngle>0 && arrowAngle<90) arrowAngle =arrowAngle+8;
                                   if(arrowAngle>-180 && arrowAngle<-90) arrowAngle =arrowAngle+8;
                                   if(arrowAngle<180 && arrowAngle>90) arrowAngle =arrowAngle-8;
                                  transform1.rotate(arrowAngle);

                                 //  сторюємо нову позначку з танормацією
                                   QPixmap *transPixmap = new QPixmap(arrowPix->transformed(transform1));

                          // встановлюємо позицію на 1,3 лінії
                                   QPointF  arrowPoint(arrow2point.x()+((point1.x()-arrow2point.x())/3),arrow2point.y()+((point1.y()-arrow2point.y())/3));
                                   ImagePoint* arrowPointI= new ImagePoint( arrowPoint.x(),  arrowPoint.y(), *transPixmap, "arrow", Point::Middle);
                                    arrow2point = point1;
                                    // додаваня до шару відображення
                                    layer1->addGeometry(arrowPointI);
                            }

                lastPoint=point1;

                 }
            // якщо є область обльту

             if (manevrPaint.hasSyrvey()==true)
             {
                  QList<Point*> *surveyPoints = new QList<Point*>;
                 QPixmap* pixm = new QPixmap(100,200);
                  pixm->load("images/Location_Tag.png");
                 //pixm->fill(Qt::transparent);     
                 QVector<PointOfTrace> pointSurveyNumber=  manevrPaint.getSurveyNumber();
                 // для кожної точки маневру
                 for( int k=0; k<pointSurveyNumber.size();k++)
                 {
                     // за потреби перретворюємо в георгрвфічні корлинати
                  //  QPointF pointSurvey=  trans1.returnGeoCoord(point2, pointSurveyNumber[k].x(), pointSurveyNumber[k].y());
                  PointOfTrace   pointSurvey = pointSurveyNumber[k];
                     ImagePoint* manevrPoint= new ImagePoint( pointSurvey.x(),  pointSurvey.y(), *pixm, QString("point").arg(i), Point::BottomMiddle);
                        surveyPoints->append(manevrPoint);
                 }
                 QPen* surveyPen = new QPen(QColor(100, 100, 0, 100));
                 surveyPen->setWidth(4);
                 //surveyPen->setStyle(Qt::DashDotLine);
                  // створюємо маневр ма шарі картиж

                     LineString* surveyString = new LineString( *surveyPoints, "", surveyPen);
                 // додаємо лінії на шар малювання
                  layer1->addGeometry(surveyString);

             }
             }

             LineString* ls = new LineString( manevrRoute, manevrPaint.ManevrName(), linepen);
             //    LineString* surveyString = new LineString( surveyPoints, "", surveyPen);
             // додаємо лінії на шар малювання
             layer1->addGeometry(ls);
         }
// додаємо точкии до осен\новного масиву
 newRoute.swap(routePoints);
       //mainPaitingManevr();
}
//
void MainWindow::manevr_Edit(){
  manevrsList= mMarch_Dialog->returManevrList();
  updatePainingManevr();
}

//
void MainWindow::keyPressEvent(QKeyEvent* event)
{
        switch (event->key()) {
        case Qt::Key_Delete: {
          //  foreach (QGraphicsItem *item, selectedItems()) {
          //      removeItem(item);
         //       delete item;
            manevrsList.removeAt(selectedManevr);
                updatePainingManevr();
            }
           // deselectItems();
            break;
        }
    }

