#ifndef POINT_DIALOG_H
#define POINT_DIALOG_H

#include <QDialog>
#include "stratecoord.h"
#include"pointt.h"

namespace Ui {
class Point_Dialog;
}

class Point_Dialog : public QDialog , public strateCoord
{
    Q_OBJECT

public:
    explicit Point_Dialog(PointOfTrace editPoint, QWidget *parent = 0, bool isStartPoint = false, QPointF strPoint = QPointF(0,0));
    ~Point_Dialog();
    PointOfTrace return_editPoint();

private:
    Ui::Point_Dialog *ui;
    PointOfTrace editetPoint;
    strateCoord trans1;
    QPointF startPoint;
    QPointF editetPointLocal;
public slots:
    void point_X_edit_Action();
    void point_Y_edit_Action();
    void updateTable();

};

#endif // POINT_DIALOG_H
