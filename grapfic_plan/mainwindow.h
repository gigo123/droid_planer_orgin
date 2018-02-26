#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QString>
#include <mapcontrol.h>
#include <osmmapadapter.h>
#include <maplayer.h>
#include <imagepoint.h>
#include <circlepoint.h>
#include <linestring.h>
#include "manevr_item.h"
#include "stratecoord.h"
#include "pointt.h"
//#include <QtSvg/QSvgGenerator>
class CircleDialog;
class Line_Dialog;
class Box_Line_Dialog;
class Baterfly_Dialog;
class Endless_Dialog;
class Snake_Dialog;
class Region_Dialog;
class VEPolyline;
class march_Dialog;
class Point_Dialog;

using namespace qmapcontrol;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow , public strateCoord
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    MapControl* mc;  // Qmap
    void addZoomButtons(); //Qmap
public:
    //explicit MainWindow(QWidget *parent = 0);
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

   // void mainPaitingM();
public slots:
    void Action_circle();
     void Action_Box_Line();
    void Action_Endless();
     void Action_Baterfly_Dialog();
     void Action_Snake_Dialog();
     void Action_Region_Dialog();
    void Action_Open_File();
    void Action_Line();
    void Action_March();
    //

    //
    void openKml();
    void saveKml();
    void set_Circle();
    void set_Line();
    void set_Box_Line();
    void set_Baterfly();
     void set_Endless();
     void set_Snake();
     void set_Region();
     void manevr_Edit();
       //
    void geometryClicked(Geometry* geom, QPoint coord_px); //Qmap
    void resizeEvent(QResizeEvent *qEvent); //Qmap
    void writeStartPoint(const QMouseEvent* eventClic, const QPointF); //qmap - точка
    void addStartPoint();  //qmap- my
    //
    void Action_addStartPoint();
     void set_startPoint();

private:
     Point_Dialog *mPoint_Dialog;
     CircleDialog  *mcirleDialog;
     Line_Dialog *mlineDialog;
     Box_Line_Dialog *mBox_Line_Dialog;
     Baterfly_Dialog *mBaterfly_Dialog;
     Endless_Dialog *mEndless_Dialog;
     Snake_Dialog *mSnake_Dialog;
     Region_Dialog *mRegion_Dialog;
     march_Dialog *mMarch_Dialog;
     int manevrNameN[9]{1,1,1,1,1,1,1,1,1};
     QString routeName;

     ImagePoint *startPoint;//
     QPointF startPointF;
     Layer *layer1;
     QVector<Layer*> layerVector;
     QGraphicsScene *painter;

     int selectedManevr;
     QString FileName1;
     QPointF endManevrPoint;
    // VEPolyline *polyline;
     QVector<VEPolyline> *RouteLines;
     QList<Point*> routePoints;
    bool startPointSetet;
    bool newPolyline;

     QList<manevr_Item> manevrsList;
    // QGraphicsItemGroup *group_1;
     //  QGraphicsItemGroup *group_Line;
      //QGraphicsItemGroup *group_point;
     //QGraphicsItemGroup *group_Circle;

     //void checkSelection();
     //void checkActionStates();
     //void selectItem(QGraphicsItem *item);
     //void selectNewItem(QGraphicsItem *item);
     void paintingManevr(QVector<PointOfTrace> listPoints, bool conectionline, bool survey, int manevrType);
     void updatePainingManevr();
    // void mainPaitingManevr();
     void keyPressEvent(QKeyEvent* event);


};

#endif // MAINWINDOW_H
