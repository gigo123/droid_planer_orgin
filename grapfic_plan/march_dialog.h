#ifndef MARCH_DIALOG_H
#define MARCH_DIALOG_H

#include <QDialog>
#include "manevr_item.h"
#include <qstringlistmodel.h>
#include <mapcontrol.h>

class Manevr_Dialog;
class MainWindow;

using namespace qmapcontrol;
namespace Ui {
class march_Dialog;
}

class march_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit march_Dialog(QList<manevr_Item> manervsList, QString name, QWidget *parent = 0);
    ~march_Dialog();
    QList<manevr_Item> *formManevrList;
 //   void setManevrList(QList<manevr_Item> &manervsList);

private:
    Ui::march_Dialog *ui;
    QStringListModel *model;
    Manevr_Dialog *mManevrEdit;
    int numberEditingManevr;
    void updateTable();
   // void add_Manevr();
QString  routeName;
  public slots:
    void remove_Manevr();
    QList<manevr_Item> returManevrList();
    void actions_manevrEdit();
    void returnEditesManevr(manevr_Item editetManevr);
    void swapManervUp();
     void swapManervDown();
};

#endif // MARCH_DIALOG_H
