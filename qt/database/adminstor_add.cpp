#include "adminstora_dd.h"
#include "ui_adminstor_add.h"
#include <QMessageBox>

AdminstorAdd::AdminstorAdd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminstorAdd)
{
    this->setWindowTitle("注册");
    ui->setupUi(this);
    database_ = DataBase::getDataBasePtr() ;
    ui->label_username_status->setText("");
    init_combox();
}

AdminstorAdd::~AdminstorAdd()
{
    delete ui;
}

void AdminstorAdd::on_pushButton_sure_clicked()
{
    QString new_user_name = ui->lineEdit_username->text()  ;
    QString new_user_password =  ui->lineEdit_password->text() ;
    int new_user_role = ui->comboBox_role->currentIndex();
    bool is_add = database_->addNewUser(new_user_name,new_user_password,new_user_role) ;
    if (is_add==true)
    {
        QMessageBox::information(NULL,"信息","用户创建成功") ;
        this->close() ;
    }
    else
    {
        QMessageBox::critical(NULL , "信息","注册失败") ;
        ui->label_username_status->setText("<font color=\"red\">用户名已存在</font>");
    }
}

void AdminstorAdd::init_combox()
{
    ui->comboBox_role->addItem("监控员");
    ui->comboBox_role->addItem("超级管理员");
    ui->comboBox_role->setCurrentIndex(0);
}
