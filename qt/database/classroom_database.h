#ifndef CLASSROOM_DATABASE_H
#define CLASSROOM_DATABASE_H

#include <QWidget>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMutex>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include "database.h"
#include <adminstora_dd.h>
#include <classroom_change.h>


namespace Ui {
class ClassRoomDataBase;
}

class ClassRoomDataBase : public QWidget
{
    Q_OBJECT

public:
    explicit ClassRoomDataBase(QWidget *parent = 0);

private:
    ~ClassRoomDataBase();
    void initTabelView() ;
public slots:
    void showTabelView() ;
    void showTabel(QModelIndex current_index) ;
    void tableGetUpdata(int update_row , QSqlRecord &update_record) ;
    void tableGetDelete(int delete_row) ;
    void tableDataChange(QModelIndex &row_index , QModelIndex &col_index) ;
    void tableGetInsert(QSqlRecord &insert_sql) ;
private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_update_clicked();

private:
    Ui::ClassRoomDataBase *ui;
    DataBase *classroom_database_ ;
    QStandardItemModel *item_model_ ;
    QSqlTableModel *table_model_ ;
    QSqlRelationalTableModel *relation_table_model_ ;
    AdminstorAdd *admin_widget_ ;
    ClassroomChange *classroomChange_widget_ ;
};

#endif // CLASSROOM_DATABASE_H
