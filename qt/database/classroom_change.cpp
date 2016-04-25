#include "classroom_change.h"
#include "ui_classroom_change.h"
#include <global_file.h>
#include <QTimer>
#include <QMessageBox>

ClassroomChange::ClassroomChange(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassroomChange)
{
    ui->setupUi(this);
    initView();
    data_base_ptr_ = DataBase::getDataBasePtr() ;
    connect(ui->comboBox_building,SIGNAL(currentIndexChanged(int)),this, SLOT(changeClassRoomIndex(int))) ;
}

ClassroomChange::~ClassroomChange()
{
    delete ui;
}

void ClassroomChange::initView()
{
    current_index_ = 0 ;
    for(int i = 0 ; i < GlobalFile::build_list_.length();i++)
    {
        ui->comboBox_building->addItem(GlobalFile::build_list_.at(i));
    }
    for(int j = 0 ; j < GlobalFile::classroom_list_[GlobalFile::build_list_[current_index_]].size() ; j++)
    {
        ui->comboBox_classroom->addItem(GlobalFile::classroom_list_[GlobalFile::build_list_[current_index_]].at(j)) ;
    }
}

//building index is changed 
void ClassroomChange::changeClassRoomIndex(int index)
{
    ui->comboBox_classroom->clear();
    current_index_ = index ;
    for(int j = 0 ; j < GlobalFile::classroom_list_[GlobalFile::build_list_[current_index_]].size() ; j++)
    {
        ui->comboBox_classroom->addItem(GlobalFile::classroom_list_[GlobalFile::build_list_[current_index_]].at(j)) ;
    }
}



void ClassroomChange::on_pushButton_sure_clicked()
{
    bool order_all_classroom = ui->checkBox_order->isChecked() ;
    QString current_building = ui->comboBox_building->currentText() ;
    QString current_classroom = ui->comboBox_classroom->currentText() ;
    QStringList classroom_list_  ;
    int time = ui->spinBox_ordertime->text().toInt();
    if(order_all_classroom==true)
    {
        for(int j = 0 ; j < GlobalFile::classroom_list_[GlobalFile::build_list_[current_index_]].size() ; j++)
        {
            data_base_ptr_->updateClassRoom(GlobalFile::classroom_list_[GlobalFile::build_list_[current_index_]].at(j),1);
            classroom_list_<<GlobalFile::classroom_list_[GlobalFile::build_list_[current_index_]].at(j) ;
        }
    }
    else
    {
        data_base_ptr_->updateClassRoom(current_classroom,1);
        classroom_list_<< current_classroom ;
    }
    change_classroom_list_ = classroom_list_ ;
    QMessageBox::information(NULL,"预定","预定成功") ;
    QTimer *t  = new QTimer(this) ;
    connect(t,SIGNAL(timeout()),this,SLOT(change_classroom_status())) ;
    t->start(time*60*1000);
}

void ClassroomChange::change_classroom_status()
{
    for(int i  = 0 ; i< change_classroom_list_.size() ; i++)
    {

        data_base_ptr_->updateClassRoom(change_classroom_list_.at(i),0);
    }
}

void ClassroomChange::on_pushButton_clicked()
{

}
