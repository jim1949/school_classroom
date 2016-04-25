#ifndef CLASSROOMTREEVIEW_H
#define CLASSROOMTREEVIEW_H

#include <QObject>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMap>
#include <database.h>
#include <QMap>


class ClassRoomTreeView : public QObject
{
    Q_OBJECT
public:
    explicit ClassRoomTreeView(QObject *parent = 0);
    bool getRoomTreeViewInit() ;
    void setTreeView(QTreeView *treeview, QStringList &horizontalHeader) ;

    void setCurrentPeople(const QString &classroom_name,const int people_num) ;
    void setBuildingName(const QStringList &building_name_list) ;
    void setClassRoomName(const QString &building_name , const QStringList &classroom_name_list) ;
    //bool setCurrentOrderPeople(int order_people_num) ;
    void refreshPeopleNum() ;

     void setViewModel() ;
signals:
    void setClassroomImage(QString classroom_name) ;
public slots:
    void setCurrentImage(QModelIndex current_index) ;

private:
    QString getBuildingName(const QString &classroom_name) ;


private:
    QTreeView *classroom_tree_view ;
    QStandardItemModel *classroom_item_model ;
    QStringList build_name_list_ ;
    DataBase *database_ ;
    QMap<QString , QStringList> classroom_list_ ;
    QMap<QString , int> classroom_row_ ;
};

#endif // CLASSROOMTREEVIEW_H
