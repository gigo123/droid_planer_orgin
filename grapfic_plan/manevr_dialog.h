#ifndef MANER_DIALOG_H
#define MANER_DIALOG_H

#include <QDialog>
#include "manevr_item.h"
#include <mapcontrol.h>

//class Manevr_Dialog;
class MainWindow;
class Point_Dialog;

using namespace qmapcontrol;

namespace Ui {
class Manevr_Dialog;
}

class Manevr_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Manevr_Dialog(manevr_Item manervs , QString name, QWidget *parent = 0);
    ~Manevr_Dialog();
    int getRepeatNumber();
  public slots:
    void point_delete_action();
    void point_accept_action();
    void point_Edit_action();
    void point_Resive();
    void updateTabble();
    void updateSpeed();
    void updateRepeatNumber();
 signals:
  void  acceptRes(manevr_Item );


private:
    Ui::Manevr_Dialog *ui;
     QVector<PointOfTrace> pointVector;
      int numberEditingPoint;
       Point_Dialog *mPoinDialog;
       QPointF startPoint;
       int manevrSpeed;
       int repeatNumber;
       manevr_Item editetManevr;
       QString manevrName;
};

#endif // MANER_DIALOG_H
