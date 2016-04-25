#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include <login_dialog.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况1

    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    LoginDialog login_dialog ;
    Widget w ;
    if(login_dialog.exec() == QDialog::Accepted)
    {
        w.show();
        return a.exec();
    }
    return 0;
}
