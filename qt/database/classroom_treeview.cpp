#include "classroom_treeview.h"
#include <QDebug>

ClassRoomTreeView::ClassRoomTreeView(QObject *parent) : QObject(parent)
{
   this->database_ = DataBase::getDataBasePtr() ;
}

void ClassRoomTreeView::setTreeView(QTreeView *treeview,QStringList &horizontalHeader)
{
    if(!treeview)
    {
        qDebug()<<"no treeview set" ;
        exit(1);
    }
    this->classroom_tree_view = treeview ;
    this->classroom_item_model = new QStandardItemModel(this->classroom_tree_view) ;
    this->classroom_item_model->setHorizontalHeaderLabels(horizontalHeader);
    this->classroom_tree_view->setModel(this->classroom_item_model);
}

bool ClassRoomTreeView::getRoomTreeViewInit()
{
    if(!classroom_tree_view)
    {
       return false ;
    }
    if(build_name_list_.empty())
    {
        return false ;
    }
    setViewModel();
    connect(this->classroom_tree_view,SIGNAL(doubleClicked(QModelIndex)),this ,SLOT(setCurrentImage(QModelIndex))) ;
}

void ClassRoomTreeView::setBuildingName(const QStringList &building_name_list)
{
    build_name_list_ = building_name_list ;
}

void ClassRoomTreeView::setClassRoomName(const QString &building_name, const QStringList &classroom_name_list)
{
    this->classroom_list_[building_name] = classroom_name_list ;
}

void ClassRoomTreeView::setViewModel()
{
    for(int i = 0 ; i < build_name_list_.length();i++)
    {
        QStandardItem *building_item = new QStandardItem(build_name_list_.at(i)) ;
        building_item->setEditable(false);
        classroom_item_model->appendRow(building_item);
        QStringList building_classroom_list = classroom_list_[build_name_list_.at(i)] ;
        for(int j = 0 ; j < building_classroom_list.length() ; j++)
        {
            QStandardItem *classroom_item = new QStandardItem(building_classroom_list.at(j)) ;
            classroom_row_[building_classroom_list.at(j)] = j ;
            classroom_item->setEditable(false);
            building_item->appendRow(classroom_item);
            int people_num = database_->getPeopleNum(building_classroom_list.at(j)) ;
            QString current_people = QString("%0/80").arg(people_num) ;
            QStandardItem *classroom_people_item = new QStandardItem(current_people) ;
            classroom_people_item->setEditable(false);
            building_item->setChild(j,1,classroom_people_item);

        }
    }

}

void ClassRoomTreeView::setCurrentPeople(const QString &classroom_name, const int people_num)
{
    QList<QStandardItem *> find_standard_item ;
    QString building_name = getBuildingName(classroom_name) ;
    find_standard_item = classroom_item_model->findItems(building_name) ;
    QString current_people = QString::number(people_num)+"/80" ;
    QStandardItem *current_people_item = new QStandardItem(current_people) ;
    qDebug()<< "update database";
    database_->updateClassRoom(classroom_name,people_num);
    //update the people num of this classroom
    current_people_item->setEditable(false);
    find_standard_item.at(0)->setChild(classroom_row_[classroom_name],1,current_people_item);
}

QString ClassRoomTreeView::getBuildingName(const QString &classroom_name)
{
    if(classroom_name.at(0)=='A') return "教西A楼" ;
    else if (classroom_name.at(0)=='B') return "教西B楼";
    else if (classroom_name.at(0)=='C')return "教西C楼";
    else if (classroom_name.at(0)=='D')return "教西D楼";

}

//change the image show in the middle
void ClassRoomTreeView::setCurrentImage(QModelIndex current_index)
{
    QString classroom_name  ;
    int current_row =  current_index.row() ;
    int current_col = current_index.column() ;
    if(1 == current_col)
    {
        QModelIndex parent_item = current_index.parent() ;
        QModelIndex classroom_index = parent_item.child(current_row,0) ;
        classroom_name = classroom_index.data().toString() ;
    }
    else
    {
        classroom_name = current_index.data().toString() ;
    }
    qDebug()<<classroom_name ;
    emit setClassroomImage(classroom_name);

}

void ClassRoomTreeView::refreshPeopleNum()
{
    for(int i = 0 ; i < this->build_name_list_.size() ; i++)
    {
        for(int j = 0; j < this->classroom_list_[build_name_list_[i]].size();j++)
        {
            QString classroom_name = this->classroom_list_[build_name_list_[i]].at(j) ;
            int people_num = database_->getPeopleNum(classroom_name) ;
            this->setCurrentPeople(classroom_name,people_num);
        }
    }
}
