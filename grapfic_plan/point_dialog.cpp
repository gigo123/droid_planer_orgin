#include "point_dialog.h"
#include "ui_point_dialog.h"
#include "stratecoord.h"

Point_Dialog::Point_Dialog(PointOfTrace editPoint, QWidget *parent, bool isStartPoint,QPointF strPoint) :
    QDialog(parent),
    ui(new Ui::Point_Dialog)
{

    ui->setupUi(this);
    editetPoint= editPoint;
    QIntValidator* intvalidator= new QIntValidator(0,10000);
       ui->height_0_edit->setValidator(intvalidator);
       ui->geo_coord_radioB->setChecked(true);
       startPoint=strPoint;
       int speed = editetPoint.getSpeed();
       ui->speed_edit->setText(QString ("%1").arg(speed));
    if(isStartPoint){
        ui->point_action_box->addItem("Старт");
         setWindowTitle("Точка початку маршруту");
         updateTable();
         ui->geo_coord_radioB->setEnabled(false);
          ui->local_coord_radioB->setEnabled(false);
   }
    else{
         setWindowTitle("Редагування точки");
         updateTable();
        connect(ui->geo_coord_radioB,&QRadioButton::toggled,[this]{Point_Dialog::updateTable();});
        connect(ui->local_coord_radioB,&QRadioButton::toggled,[this]{Point_Dialog::updateTable();});
    }
    connect(ui->point_X_edit,&QLineEdit::editingFinished,this,&Point_Dialog::point_X_edit_Action);
     connect(ui->point_Y_edit,&QLineEdit::editingFinished,this,&Point_Dialog::point_Y_edit_Action);

}

Point_Dialog::~Point_Dialog()
{
    delete ui;
}
//
void Point_Dialog::point_X_edit_Action(){
if(ui->geo_coord_radioB->isChecked()==true){
    editetPoint.setPointG(QPointF(ui->point_X_edit->text().toDouble(),editetPoint.y()));
    //editetPoint.setX( ui->point_X_edit->text().toDouble());
   // editetPointLocal= trans1.returnStrateCoord(startPoint,editetPoint);
    }
    else {
    editetPointLocal = editetPoint.getLocalPoint();
     editetPoint.setPointlocal(QPointF(ui->point_X_edit->text().toDouble(),editetPointLocal.y()));
   // editetPointLocal.setX( ui->point_X_edit->text().toDouble());
    // editetPoint= trans1.returnGeoCoord(startPoint,editetPointLocal.x(),editetPointLocal.y());
}
}
//
void Point_Dialog::point_Y_edit_Action(){
    if(ui->geo_coord_radioB->isChecked()==true){
        editetPoint.setPointG(QPointF(editetPoint.x(),ui->point_Y_edit->text().toDouble()));
    //editetPoint.setY( ui->point_Y_edit->text().toDouble());
     // editetPointLocal= trans1.returnStrateCoord(startPoint,editetPoint);
    }
    else {
        editetPointLocal = editetPoint.getLocalPoint();
         editetPoint.setPointlocal(QPointF(editetPointLocal.x(),ui->point_Y_edit->text().toDouble()));
        //editetPointLocal.setY( ui->point_X_edit->text().toDouble());
         //editetPoint= trans1.returnGeoCoord(startPoint,editetPointLocal.x(),editetPointLocal.y());
    }
}

void Point_Dialog::updateTable()
{
    QLocale locale(QLocale::English);
    QDoubleValidator* validatorX =  new QDoubleValidator( -180, 180, 8 );
    QDoubleValidator* validatorY =  new QDoubleValidator( -90, 90, 8 );
     QIntValidator* intvalidatorX= new QIntValidator(-100000,100000);
     validatorX->setNotation(QDoubleValidator::StandardNotation);
     validatorY->setNotation(QDoubleValidator::StandardNotation);
     locale.setNumberOptions(QLocale::RejectGroupSeparator);
      validatorX->setLocale(locale);\
      validatorY->setLocale(locale);
    if(ui->geo_coord_radioB->isChecked()==true){
       // editetPoint= trans1.returnGeoCoord(startPoint,editetPointLocal.x(),editetPointLocal.y());
    QString tableX  = QString::number( editetPoint.x(),'f',8);
     QString tableY  = QString::number( editetPoint.y(),'f',8);
    ui->point_X_edit->setText(tableX);
    ui->point_Y_edit->setText(tableY);
     ui->point_X_edit->setValidator(validatorX);
     ui->point_Y_edit->setValidator(validatorY);

     ui->label_set_x->setText("Довгота градуси");
     ui->label_set_y->setText("Широта градуси");
    }
    else {
       //editetPointLocal= trans1.returnStrateCoord(startPoint,editetPoint);
        editetPointLocal = editetPoint.getLocalPoint();
        QString tableX  = QString::number( editetPointLocal.x(),'f',0);
         QString tableY  = QString::number( editetPointLocal.y(),'f',0);
        ui->point_X_edit->setText(tableX);
        ui->point_Y_edit->setText(tableY);
         ui->point_X_edit->setValidator(intvalidatorX);
         ui->point_Y_edit->setValidator(intvalidatorX);
         ui->label_set_x->setText("Локальний Х м");
         ui->label_set_y->setText("Локальний Y м");
    }

}
//
PointOfTrace Point_Dialog::return_editPoint(){
    return editetPoint;

}
