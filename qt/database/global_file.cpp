#include "global_file.h"

GlobalFile::GlobalFile()
{

}

int GlobalFile::CURRENT_USER_ROLE = -1 ;
QMap<QString,QStringList> GlobalFile::classroom_list_  ;
QStringList GlobalFile::build_list_  ;
