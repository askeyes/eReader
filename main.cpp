#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QApplication>
#include "ereader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    QPixmap pixmap(":/res/splash.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    EReader w;


    QTimer::singleShot(1000, splash, SLOT(close()));
    QTimer::singleShot(1100, &w, SLOT(show()));

    a.setWindowIcon(QIcon("ebook.ico"));

    //QApplication::desktop()->availableGeometry();
    return a.exec();
}
