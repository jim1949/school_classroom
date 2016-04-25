#ifndef CLASSROOM_CHANGE_H
#define CLASSROOM_CHANGE_H

#include <QWidget>
#include <database.h>

namespace Ui {
class ClassroomChange;
}

class ClassroomChange : public QWidget
{
    Q_OBJECT

public:
    explicit ClassroomChange(QWidget *parent = 0);
    ~ClassroomChange();

private:
    void initView() ;
private slots:
    void changeClassRoomIndex(int index) ;
    void on_pushButton_clicked();
    void change_classroom_status() ;
    void on_pushButton_sure_clicked();

private:
    Ui::ClassroomChange *ui;
    int current_index_ ;
    DataBase *data_base_ptr_ ;
    bool is_time_over ;
    int order_staus_ ;
    QStringList change_classroom_list_ ;
};

#endif // CLASSROOM_CHANGE_H
