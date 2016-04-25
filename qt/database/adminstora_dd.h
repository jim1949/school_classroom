#ifndef ADMINSTORA_DD_H
#define ADMINSTORA_DD_H

#include <QWidget>
#include <database.h>

namespace Ui {
class AdminstorAdd;
}

class AdminstorAdd : public QWidget
{
    Q_OBJECT

public:
    explicit AdminstorAdd(QWidget *parent = 0);
    ~AdminstorAdd();    
private slots:
    void on_pushButton_sure_clicked();
    void init_combox() ;
private:
    Ui::AdminstorAdd *ui;
    DataBase *database_ ;
};

#endif // ADMINSTORA_DD_H
