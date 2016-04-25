#include "classroom_database.h"
#include "ui_classroom_database.h"
#include <QDebug>
#include <QMessageBox>


ClassRoomDataBase::ClassRoomDataBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassRoomDataBase)
{
    ui->setupUi(this);
    this->setWindowTitle("数据库管理");
    classroom_database_ = DataBase::getDataBasePtr() ;
}

ClassRoomDataBase::~ClassRoomDataBase()
{
    delete ui;
}


void ClassRoomDataBase::initTabelView()
{
    QStringList table_list =  classroom_database_->getTablesName();
    item_model_ = new QStandardItemModel(ui->treeView_table) ;
    QStringList HorizontalHeaderLabels ;
    HorizontalHeaderLabels<<"表名" ;
    item_model_->setHorizontalHeaderLabels(HorizontalHeaderLabels);
    ui->treeView_table->setModel(item_model_);
    for(int i = 0 ; i < table_list.length() ; i++)
    {
        QStandardItem *item = new QStandardItem(table_list.at(i)) ;
        item->setEditable(false);
        item_model_->appendRow(item);
    }
    this->table_model_ = new QSqlTableModel(ui->treeView_table) ;
    this->relation_table_model_ = new QSqlRelationalTableModel(ui->treeView_table) ;
    connect(ui->treeView_table,SIGNAL(doubleClicked(QModelIndex)),
            this , SLOT(showTabel(QModelIndex))) ;

}

void ClassRoomDataBase::showTabelView()
{
    initTabelView();
    this->show();
}

void ClassRoomDataBase::showTabel(QModelIndex current_index)
{
    qDebug()<<"SHOW TABLE"<<" "<<current_index.data().toString() ;
    table_model_->setTable(current_index.data().toString());
    if(table_model_->select())
    {
        ui->tableView_database->setModel(table_model_);
    }
    else
    {
        QMessageBox::warning(NULL , "warning" , "没有数据",QMessageBox::Yes|
                             QMessageBox::No) ;
    }
}


//void ClassRoomDataBase::getTableChangeData(QModelIndex row_index, QModelIndex col_index)
//{
//    qDebug()<<"get table changed";
//    qDebug()<<row_index.data().toString()<<" "<<col_index.data().toString() ;
//    qDebug()<<row_index.row()<<" "<<row_index.column() ;
//}


void ClassRoomDataBase::tableDataChange(QModelIndex &row_index, QModelIndex &col_index)
{

}

void ClassRoomDataBase::tableGetDelete(int delete_row)
{

}

void ClassRoomDataBase::tableGetInsert(QSqlRecord &insert_sql)
{

}

void ClassRoomDataBase::tableGetUpdata(int update_row, QSqlRecord &update_record)
{

}

void ClassRoomDataBase::on_pushButton_search_clicked()
{
   admin_widget_ = new AdminstorAdd() ;
   admin_widget_->show();
}

void ClassRoomDataBase::on_pushButton_update_clicked()
{
    classroomChange_widget_ = new ClassroomChange() ;
    classroomChange_widget_->show();
}
