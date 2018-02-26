#include "manevr_dialog.h"
#include "ui_maner_dialog.h"
#include "manevr_item.h"
#include "point_dialog.h"
#include <qmath.h>

Manevr_Dialog::Manevr_Dialog(manevr_Item manervs, QString name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Manevr_Dialog)
{
    ui->setupUi(this);

    QIntValidator *intValidator= new QIntValidator(1,65000,this);
   ui->speed_edit->setValidator(intValidator);
    ui->number_repeat_edit->setValidator(intValidator);
editetManevr = manervs;
 startPoint = manervs.getGlobalStartPoint();
    connect(ui->point_delete_button,&QPushButton::pressed,this,&Manevr_Dialog::point_delete_action);
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&Manevr_Dialog::point_accept_action);
    connect(ui->point_edit_button,&QPushButton::pressed,this,&Manevr_Dialog::point_Edit_action,Qt::UniqueConnection);
    connect(ui->buttonBox,&QDialogButtonBox::rejected,[this]{Manevr_Dialog::reject();});
    connect(ui->geo_coord_radioB,&QRadioButton::toggled,[this]{Manevr_Dialog::updateTabble();});
    connect(ui->local_coord_radioB,&QRadioButton::toggled,[this]{Manevr_Dialog::updateTabble();});
    connect(ui->speed_edit,&QLineEdit::editingFinished,this,&Manevr_Dialog::updateSpeed);
    connect(ui->number_repeat_edit,&QLineEdit::textEdited,this,&Manevr_Dialog::updateRepeatNumber);
    ui->route_name_edit->setText(name);
    repeatNumber = manervs.getRepeatNumber();
    ui->number_repeat_edit->setText(QString ("%1").arg(repeatNumber));

    ui->tableWidget->setColumnCount(2); //задємо кількість колонок
    ui->tableWidget->setShowGrid(true); // вствновлюємо показ сітки
    // можливість вибирати еленти тільки по одному
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // можлтвість вибирати тільки порядково
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "X"<<"Y");



    // отримуємлон номери точток маневру
   pointVector=  manervs.getPointsNumber();
  //  pointVectorLocal=  manervs.getPointsNumberlocal();
   // встановлюємо нові точки маневру
ui->name_edit->setText(manervs.ManevrName());
ui->geo_coord_radioB->setChecked(true);
  // выдображення в таблицы
manevrName=name;
    updateTabble();
   manevrSpeed =100;
   ui->speed_edit->setText(QString("%1").arg(manevrSpeed));

}
//
Manevr_Dialog::~Manevr_Dialog()
{
    delete ui;
}

int Manevr_Dialog::getRepeatNumber()
{
    return repeatNumber;
}

void Manevr_Dialog::point_delete_action()
{

     QVector<int>  selectedRows;
   // шукаємо виділену точку
       foreach (QModelIndex index, ui->tableWidget->selectionModel()->selectedRows()){
           selectedRows.append(index.row());
       }
       // якщо немає виділеної точки - видаляємо останню
       if (selectedRows.capacity() ==0 )
       {
            ui->tableWidget->removeRow(pointVector.size() -1);
            // видалення з масиву точок
          pointVector.removeLast();
       }
       else
       {
           foreach(int row, selectedRows){
               ui->tableWidget->removeRow(row);
                 pointVector.remove(row);
           }
       }
       //якщо точок немає - зробити видалення недоступним
       if(pointVector.size() == 1){
            ui->point_delete_button->setEnabled(false);

            // повыдомлення про останню точку.
    }

}
//
void Manevr_Dialog::point_accept_action(){
    editetManevr.setPointsNumber(pointVector);
    editetManevr.setRepeatNumber(repeatNumber);
    editetManevr.setManevName(ui->name_edit->text());
     emit accept();
    emit acceptRes(editetManevr);
}
void Manevr_Dialog::point_Edit_action(){


    //
     QVector<int>  selectedRows;
    PointOfTrace editingPoint;
   // шукаємо виділену точку
       foreach (QModelIndex index, ui->tableWidget->selectionModel()->selectedRows()){
           selectedRows.append(index.row());
       }
       // якщо немає виділеної точки - видаляємо останню
       if (selectedRows.capacity() !=0 )
       {
            numberEditingPoint= selectedRows[0];
        editingPoint=pointVector[ numberEditingPoint]; 
        mPoinDialog = new Point_Dialog( editingPoint,this,false,startPoint);
       connect(mPoinDialog,&Point_Dialog::accepted,this,&Manevr_Dialog::point_Resive);
       mPoinDialog->show();
       }
}

void Manevr_Dialog::point_Resive(){
    PointOfTrace resivePoint;

    resivePoint = mPoinDialog->return_editPoint();  
    if(editetManevr.ManevrName()=="Старт"){
        if(numberEditingPoint ==0 && resivePoint!=pointVector[0]){
            QPointF newSpoint(resivePoint.x(),resivePoint.y());
          //  for (int i=1;i<pointVector.size();i++) pointVector[i].setStartPoint(newSpoint);
            editetManevr.setglobalStratPointStart(newSpoint);
            resivePoint.setStartPoint(newSpoint);
        }

    }
        pointVector.removeAt(numberEditingPoint);
        pointVector.insert(numberEditingPoint,resivePoint);
         disconnect(mPoinDialog,&Point_Dialog::accepted,this,&Manevr_Dialog::point_Resive);
         mPoinDialog->close();
    mPoinDialog->deleteLater();
    updateTabble();
}

void Manevr_Dialog::updateTabble()
{

    ui->tableWidget->clear();
    if(ui->geo_coord_radioB->isChecked()==true){
        // для кожної точки маневру
        for( int j=0; j<pointVector.size();j++)
        {
              // выдображення в таблицы

                // знаходимо ыіндекс за розміром масиву
                int PRvectorSize = j ;
                // задаємо кількість стопчиків
                ui->tableWidget->setRowCount(PRvectorSize );
                ui->tableWidget->insertRow(PRvectorSize);

                //вивівд занчень,в строковому форматі
                QString tableX  = QString::number(pointVector[PRvectorSize].x(),'f',8);
                 QString tableY  = QString::number(pointVector[PRvectorSize].y(),'f',8);
                //ui->tableWidget->setItem(PRvectorSize  ,0, new QTableWidgetItem(tr("%1").arg(pointVector[PRvectorSize].x())));
                ui->tableWidget->setItem(PRvectorSize, 0 , new QTableWidgetItem(tableX));
                ui->tableWidget->setItem(PRvectorSize, 1 , new QTableWidgetItem(tableY));
                // мастабування ширини
                 ui->tableWidget->resizeColumnsToContents();
                 // зробиити активною кнопку видалення
        }
    }
    else{
        for( int j=0; j<pointVector.size();j++)
        {
              // выдображення в таблицы

                // знаходимо ыіндекс за розміром масиву
                int PRvectorSize = j ;
                // задаємо кількість стопчиків
                ui->tableWidget->setRowCount(PRvectorSize );
                ui->tableWidget->insertRow(PRvectorSize);
               QPointF localpoint =   pointVector[j].getLocalPoint();
                //вивівд занчень,в строковому форматі
                QString tableX  = QString::number(localpoint.x(),'f',1);
                 QString tableY  = QString::number(localpoint.y(),'f',1);
                ui->tableWidget->setItem(PRvectorSize, 0 , new QTableWidgetItem(tableX));
                ui->tableWidget->setItem(PRvectorSize, 1 , new QTableWidgetItem(tableY));
                // мастабування ширини
                 ui->tableWidget->resizeColumnsToContents();
                 // зробиити активною кнопку видалення
        }

    }

}

void Manevr_Dialog::updateSpeed()
{
    manevrSpeed = ui->speed_edit->text().toInt();

    for (int i=0;i<pointVector.size();i++){
        PointOfTrace editedPoint = pointVector.at(i);
        pointVector.removeAt(i);
        editedPoint.setSpeed(manevrSpeed);
        pointVector.insert(i,editedPoint);
    }

}

void Manevr_Dialog::updateRepeatNumber()
{
   repeatNumber = ui->number_repeat_edit->text().toInt();

}
