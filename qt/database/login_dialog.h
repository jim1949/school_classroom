#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <database.h>
#include <global_file.h>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void on_pushButton_login_clicked();
//    void on_window_close() ;
    void on_pushButton_cancel_clicked();

private:
    Ui::LoginDialog *ui;
    DataBase *data_base_ ;
};

#endif // LOGIN_DIALOG_H
