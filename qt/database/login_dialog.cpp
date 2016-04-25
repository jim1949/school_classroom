#include "login_dialog.h"
#include "ui_login_dialog.h"
#include <QDebug>
#include <QMovie>
#include <QDebug>
#include <QMessageBox>

//the login window
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    data_base_ = DataBase::getDataBasePtr() ;
    this->setWindowTitle("请先登录");
    QMovie *movie = new QMovie("/home/liao/Documents/classroom_info/login.gif") ;
    movie->setScaledSize(QSize(400,131));
    ui->label_movie->setMovie(movie);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    qDebug()<<"start" ;
    this->setWindowFlags(Qt::WindowCloseButtonHint);
//    connect(this,SIGNAL(finished(int)),this,SLOT(on_window_close())) ;
    movie->start();
    //the gif moive start
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

//to login
void LoginDialog::on_pushButton_login_clicked()
{
    QString user_name = ui->lineEdit_username->text() ;
    QString password = ui->lineEdit_password->text() ;
    if(data_base_->isUserLogin(user_name,password))
    {
        qDebug()<<"accept" ;
        accept();
    }
    else
    {
        QMessageBox::critical(NULL,"登录失败","请检查你的账户信息") ;
    }
}

void LoginDialog::on_pushButton_cancel_clicked()
{
    close() ;

}


