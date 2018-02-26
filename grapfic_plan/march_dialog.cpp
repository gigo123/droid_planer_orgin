#include "march_dialog.h"
#include "ui_march_dialog.h"
#include "manevr_dialog.h"


march_Dialog::march_Dialog(QList<manevr_Item> manervsList, QString name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::march_Dialog)
{
    ui->setupUi(this);


QList<manevr_Item> *tempManevrList ;
        tempManevrList= &manervsList;
        formManevrList = new  QList<manevr_Item>;
for(int i=0; i< tempManevrList->size();i++) {
    manevr_Item manevr1 = tempManevrList->at(i);
    formManevrList->append(manevr1);
}

routeName=name;
ui->manevr_name_edit->setText(routeName);
ui->route_number_edit->setText("1");


    // Create model
       model = new QStringListModel(this);

       // Make data

connect(ui->delete_Button,&QPushButton::pressed,this,&march_Dialog::remove_Manevr);
connect(ui->edit_button,&QPushButton::pressed,this,&march_Dialog::actions_manevrEdit,Qt::UniqueConnection);
connect(ui->manevr_name_edit,&QLineEdit::editingFinished,[this]{routeName=ui->manevr_name_edit->text();});
connect(ui->up_button,&QPushButton::pressed,this,&march_Dialog::swapManervUp);
connect(ui->down_button,&QPushButton::pressed,this,&march_Dialog::swapManervDown);
       // Add additional feature so that
       // we can manually modify the data in ListView
       // It may be triggered by hitting any key or double-click etc.
        ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
       ui->listWidget->setEditTriggers(QAbstractItemView::AnyKeyPressed |
                                                                QAbstractItemView::DoubleClicked);
updateTable();

}

march_Dialog::~march_Dialog()
{
    delete ui;
}
// оновлення таблиці
void march_Dialog::updateTable()
{
    QStringList List;
    for(int i=0; i< formManevrList->size();i++) {
        manevr_Item manevr1 = formManevrList->at(i);
        List.append(manevr1.ManevrName());
    }
     ui->listWidget->clear();
    ui->listWidget->insertItems(0,List);
    //якщо точок немає - зробити видалення недоступним
    if(List.size() == 0){
         ui->delete_Button->setEnabled(false);
 }

}




/*void march_Dialog::add_point(){
    if( PoinXchanged == true && PoinYchanged == true){

      QPointF PointR; // змвнн для задання очок
       PointR.setX(ui->point_x_edit->text().toInt());
       PointR.setY(ui->point_y_edit->text().toInt());

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
    }
    else {
       QMessageBox::critical(this,"помилка","не введо кординати точки");
    }
}
*/
//виделення точки з таблиці

void march_Dialog::remove_Manevr() {
// множина точокж
    QSet<int> selectedRows;
   // шукаємо виділену точку
       foreach (QModelIndex index, ui->listWidget->selectionModel()->selectedRows()){
           selectedRows.insert(index.row());
       }
       // якщо немає виділеної точки - видаляємо останню
       if (selectedRows.capacity() ==0 )
       {
            // видалення з масиву точок
          formManevrList->removeLast();

       }
       else
       {
           foreach(int row, selectedRows){
                formManevrList->removeAt(row);
           }


       }


}
//
QList<manevr_Item>  march_Dialog::returManevrList(){
    return *formManevrList;

}
//
void march_Dialog::actions_manevrEdit(){

    // множина точокж
        QVector<int> selectedRows;
       // шукаємо виділену точку
           foreach (QModelIndex index, ui->listWidget->selectionModel()->selectedRows()){
               selectedRows.append(index.row());
           }
 if (selectedRows.capacity() !=0 ){
    numberEditingManevr=selectedRows[0];
        manevr_Item manevrPaint = formManevrList->at(numberEditingManevr);
        // отримуємлон номери точток маневру


    mManevrEdit = new Manevr_Dialog (manevrPaint,routeName,this);
   connect(mManevrEdit,&Manevr_Dialog::acceptRes,this,&march_Dialog::returnEditesManevr);

    mManevrEdit->show();
 }

}
void march_Dialog::returnEditesManevr(manevr_Item editetManevr){

    QPointF newSpoint;
    bool pointSChanged = false;
        QList<manevr_Item> *tempManevrList  = new QList<manevr_Item>;
            if(numberEditingManevr==0){
                manevr_Item manevr2 = formManevrList->at(numberEditingManevr);
                if(manevr2.getGlobalStartPoint()!=editetManevr.getGlobalStartPoint()) {
                    pointSChanged=true;
                    //newSpoint=manevr2.getGlobalStartPoint();
                    newSpoint=editetManevr.getGlobalStartPoint();
                }
            }
        for(int i=0; i< formManevrList->size();i++) {
            manevr_Item manevr1 = formManevrList->at(i);
            if(numberEditingManevr==i){
               manevr1 = editetManevr;

            }
            else {
                if(pointSChanged==true){
                    manevr1.setglobalStratPoint(newSpoint);
                }
            }
             tempManevrList->append(manevr1);

        }

        formManevrList->clear();

        for(int i=0; i< tempManevrList->size();i++) {
            manevr_Item manevr1 = tempManevrList->at(i);
            formManevrList->append(manevr1);
        }

        mManevrEdit->close();
        mManevrEdit->deleteLater();
        updateTable();
}

void march_Dialog::swapManervDown(){

    // множина точокж
            QVector<int> selectedRows;
           // шукаємо виділену точку
               foreach (QModelIndex index, ui->listWidget->selectionModel()->selectedRows()){
                   selectedRows.append(index.row());
               }
     if (selectedRows.capacity() !=0 ){
        numberEditingManevr=selectedRows[0];
            if(numberEditingManevr!=formManevrList->size()){
                manevr_Item swapManers = formManevrList->at(numberEditingManevr);
                // отримуємлон номери точток маневру
                 formManevrList->removeAt(numberEditingManevr);
                 formManevrList->insert(numberEditingManevr+1,swapManers);
                 //formManevrList->removeAt(numberEditingManevr+1);
                 updateTable();
            }

         }
}
//
void march_Dialog::swapManervUp()

{
    // множина точокж
        QVector<int> selectedRows;
       // шукаємо виділену точку
           foreach (QModelIndex index, ui->listWidget->selectionModel()->selectedRows()){
               selectedRows.append(index.row());
           }
 if (selectedRows.capacity() !=0 ){
    numberEditingManevr=selectedRows[0];
        if(numberEditingManevr!=0){
            manevr_Item swapManers = formManevrList->at(numberEditingManevr);
            // отримуємлон номери точток маневру
            formManevrList->removeAt(numberEditingManevr);
             formManevrList->insert(numberEditingManevr-1,swapManers);
             updateTable();
        }

     }

}
