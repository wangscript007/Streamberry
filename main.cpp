#include <QtGui/QApplication>
#include "mainwindow.h"
#include "utilities.h"
#include "database.h"
#include <QThread>
#include "streamfile.h"
#include <QDebug>
#include "database.h"
#include "sbexception.h"
#include "filescan.h"
#include "playlist.h"
#include "filemeta.h"
#include "beaconsender.h"
#include "beaconreceiver.h"
#include "player.h"
#include <QCleanlooksStyle>
#include <newnetworking.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef Q_WS_X11
    //fix styling on linux
    a.setStyle(new QCleanlooksStyle());
#endif


#ifdef Q_WS_WIN
    QString execpath = a.applicationDirPath();
#else
    QString execpath(argv[0]);
    execpath.resize(execpath.lastIndexOf('/'));
#endif

    Utilities util(execpath);

    Database db;
    Player player;

    Filescan fs(db);
    QThread fsthread;
    fs.moveToThread(&fsthread);
    fsthread.start();

    BeaconSender *bs = new BeaconSender(db);
    QThread *bsthread = new QThread(&a);
    bs->moveToThread(bsthread);
    bsthread->start();

    BeaconReceiver *br = new BeaconReceiver(db);
    QThread *brthread = new QThread(&a);
    br->moveToThread(brthread);
    brthread->start();

    QObject::connect(&a, SIGNAL(aboutToQuit()), bs, SLOT(sendOfflineBeacon()));

    MainWindow w(util, db, player, fs, a);

    w.show();

    StreamFile stream(player);
    //stream.addStream("/Users/Robbie/Music/Albums/Biffy Clyro - Only Revolutions/Biffy Clyro - Many Of Horror.mp3", "test", "127.0.0.1");
    //qDebug() << stream.getStreamLength("test");

    
    return a.exec();


}
