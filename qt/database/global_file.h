#ifndef GLOBALFILE_H
#define GLOBALFILE_H

#include <QStringList>
#include <QMap>

class GlobalFile
{
public:
    GlobalFile();
    static int CURRENT_USER_ROLE  ;
    static QStringList build_list_ ;
    static QMap<QString,QStringList> classroom_list_ ;
};

#endif // GLOBALFILE_H
