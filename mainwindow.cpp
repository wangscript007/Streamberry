#include "mainwindow.h"
#include <QtGui>
#include <QDebug>
#include <QGridLayout>
#include "sidebarcontroller.h"
#include "albumpanel.h"
#include "librarycontroller.h"
#include "utilities.h"
#include "database.h"
#include "filescan.h"
#include <QSystemTrayIcon>
#include "settingsdialog.h"
#include <plsmartnew.h>
#include <plnormalnew.h>

#define TOPBARHEIGHT 26
#define BOTTOMBARHEIGHT 90

MainWindow::MainWindow(Utilities& utilities, Database &datab, Player &p, Filescan &fsinit, QApplication& aapp, QWidget *parent)
  : QMainWindow(parent), util(utilities), db(datab), player(p), fs(fsinit), app(aapp)
{
  //set window properties
  menubar = createMenuBar();

  this->setWindowTitle("Streamberry");
 
  setStyleSheet(util.getStylesheet());

  QString temp;
  if((temp = db.getSetting("windowSize")) != NULL)
  {

    QStringList list = temp.split('|');
    if(list.size()==2)
    {
      resize(list.at(0).toInt(), list.at(1).toInt());
    }
    else
    {
      resize(900, 625);
    }
  }
  else
  {
    resize(900, 625);
  }

  if((temp = db.getSetting("windowPos")) != NULL)
  {
    QStringList list = temp.split('|');
    if(list.size()==2)
    {
      move(list.at(0).toInt(), list.at(1).toInt());
    }
  }

  //initialise window layout
  centralwidget = new QWidget();
  centralwidget->setObjectName("centralWidget");
  centralwidget->setStyleSheet(util.getStylesheet());
  mainlayout = new QGridLayout(centralwidget);
  mainlayout->setMargin(0);
  mainlayout->setSpacing(0);
  initialiseGrid();

  //initialise controllers and add widgets to window
  topbarcontroller = new TopbarController(util);
  librarycontroller = new LibraryController(util, db, player, topbarcontroller->getSearchbar());
  sidebarcontroller = new SidebarController(util, db, librarycontroller);
  playbackcontroller = new PlaybackController(util, player);

  mainlayout->addWidget(topbarcontroller->getWidget(), 0, 1, 1, 1);
  mainlayout->addWidget(sidebarcontroller->getWidget(), 0, 0, 2, 1);
  mainlayout->addWidget(librarycontroller->getWidget(), 1, 1);
  mainlayout->addWidget(playbackcontroller->getWidget(), 2, 0, 1, 2);


  QObject::connect(playbackcontroller, SIGNAL(nextFile()), librarycontroller, SLOT(playNextFile()));
  QObject::connect(playbackcontroller, SIGNAL(prevFile()), librarycontroller, SLOT(playPrevFile()));
  QObject::connect(topbarcontroller, SIGNAL(musicVideoCheckStateChanged(int)), librarycontroller, SLOT(musicVideoFilter(int)));
  QObject::connect(librarycontroller, SIGNAL(songInfoData(QString,QString,QString,QString)), playbackcontroller->getSongInfo(), SLOT(updateLabels(QString,QString,QString,QString)));
  QObject::connect(&fs, SIGNAL(finishedFileScan()), librarycontroller, SLOT(updateLibrary()));
  QObject::connect(this, SIGNAL(filescanRequest()), &fs, SLOT(scan()));

  QString iconpath = util.getExecutePath();
  iconpath += "images/icon.ico";
  this->setWindowIcon((QIcon(iconpath)));
  makeTrayIcon();
  this->setMenuBar(menubar);
  setCentralWidget(centralwidget);
}

void MainWindow::makeTrayIcon()
{
  QString iconpath = util.getExecutePath();
  iconpath += "images/icon.ico";
  trayicon = new QSystemTrayIcon(QIcon(iconpath));
  QObject::connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::initialiseGrid()
{
  mainlayout->setMargin(0);
  mainlayout->setSpacing(0);
  mainlayout->setRowMinimumHeight(0, TOPBARHEIGHT);
  mainlayout->setRowStretch(0, 0);
  mainlayout->setRowStretch(1, 1);
  mainlayout->setRowMinimumHeight(2, BOTTOMBARHEIGHT);
  mainlayout->setRowStretch(2, 0);
  mainlayout->setColumnMinimumWidth(0, 190);
  mainlayout->setColumnStretch(0, 0);
  mainlayout->setColumnStretch(1, 1);
}

QMenuBar* MainWindow::createMenuBar()
{

    QMenuBar* menubar = new QMenuBar();
    menubar->setNativeMenuBar(true);
    QMenu* menus[6];
    QAction* actions[23];
    menus[0] = menubar->addMenu("File");
    menus[1] = menubar->addMenu("Edit");
    menus[2] = menubar->addMenu("Controls");
    menus[3] = menubar->addMenu("View");
    menus[4] = menubar->addMenu("Window");
    menus[5] = menubar->addMenu("Help");

    actions[0] = menus[0]->addAction("Exit Streamberry");


    actions[1] = menus[1]->addAction("Scan Folders for Media");
//    actions[2] = menus[1]->addAction("Fresh Scan for Media");
    actions[3] = menus[1]->addAction("Add Individual File");
    menus[2]->addSeparator();
  actions[20] = menus[1]->addAction("New Playlist");
  actions[21] = menus[1]->addAction("New Smart Playlist");
  menus[1]->addSeparator();
  actions[5] = menus[1]->addAction("Settings");

  actions[6] = menus[2]->addAction("Play/Pause");
  menus[2]->addSeparator();
  actions[8] = menus[2]->addAction("Next Track");
  actions[9] = menus[2]->addAction("Previous Track");
  menus[2]->addSeparator();
  actions[11] = menus[2]->addAction("Volume Up");
  actions[12] = menus[2]->addAction("Volume Down");
  menus[2]->addSeparator();
  actions[13] = menus[2]->addAction("Toggle Repeat");
  actions[14] = menus[2]->addAction("Toggle Shuffle");

  actions[15] = menus[3]->addAction("Show File Providers");
  actions[16] = menus[3]->addAction("Show Offline Files");

  actions[17] = menus[4]->addAction("Maximise");
  actions[18] = menus[4]->addAction("Minimise");
  actions[19] = menus[4]->addAction("Minimise to Tray");

  //actions[20] = menus[5]->addAction("Documentation");
  //actions[21] = menus[5]->addAction("About Streamberry");



  QObject::connect(actions[0], SIGNAL(triggered()), &app, SLOT(quit()) );

  QObject::connect(actions[1], SIGNAL(triggered()), this, SLOT(menuScan()));

//  QObject::connect(actions[2], SIGNAL(triggered()), this, SLOT(menuCleanScan()));

  QObject::connect(actions[3], SIGNAL(triggered()), this, SLOT(menuAddFile()));

  QObject::connect(actions[5], SIGNAL(triggered()), this, SLOT(menuSettings()));



  //QObject::connect(actions[6], SIGNAL(triggered()), player, SLOT(playControl()));
  /* QObject::connect(actions[7], SIGNAL(triggered()), player, SLOT(menuExitStreamberry()));
  QObject::connect(actions[8], SIGNAL(triggered()), player, SLOT(menuExitStreamberry()));
  QObject::connect(actions[9], SIGNAL(triggered()), player, SLOT(menuExitStreamberry()));
  QObject::connect(actions[10], SIGNAL(triggered()), player, SLOT(menuExitStreamberry()));
  QObject::connect(actions[11], SIGNAL(triggered()), player, SLOT(menuExitStreamberry()));
  QObject::connect(actions[12], SIGNAL(triggered()), player, SLOT(menuExitStreamberry()));
*/
  QObject::connect(actions[15], SIGNAL(triggered()), this, SLOT(menuShowFileProviders()));
  QObject::connect(actions[16], SIGNAL(triggered()), this, SLOT(menuShowOfflineFiles()));

  QObject::connect(actions[17], SIGNAL(triggered()), this, SLOT(menuMaximise()));
  QObject::connect(actions[18], SIGNAL(triggered()), this, SLOT(menuMinimise()));
  QObject::connect(actions[19], SIGNAL(triggered()), this, SLOT(menuMinimiseToTray()));

 QObject::connect(actions[20], SIGNAL(triggered()), this, SLOT(menuNewPlaylist()));
  QObject::connect(actions[21], SIGNAL(triggered()), this, SLOT(menuNewSmartPlaylist()));

  return menubar;
}

void MainWindow::resizeEvent(QResizeEvent *resize)
{
  QSize size = resize->size();
  QString winsize = QString::number(size.width());
  winsize += "|";
  winsize += QString::number(size.height());
  db.storeSetting("windowSize", winsize);

  sidebarcontroller->updateplaylistbar( (int)(size.height()/89.25) );
}

void MainWindow::moveEvent(QMoveEvent *move)
{
  QPoint pos = move->pos();
  QString winpos = QString::number(pos.x());
  winpos += "|";
  winpos += QString::number(pos.y());
  db.storeSetting("windowPos", winpos);
}

void MainWindow::menuScan()
{
  emit filescanRequest();
}

void MainWindow::menuAddFile()
{
}

void MainWindow::menuSettings()
{
    SettingsDialog settingsdialog(db, this);
    settingsdialog.setModal(true);
    settingsdialog.show();
    settingsdialog.exec();
}

void MainWindow::menuMaximise()
{
  this->setWindowState(Qt::WindowMaximized);
}

void MainWindow::menuMinimise()
{
  this->setWindowState(Qt::WindowMinimized);
}

void MainWindow::menuShowFileProviders()
{
}

void MainWindow::menuShowOfflineFiles()
{
}

void MainWindow::menuMinimiseToTray()
{
  trayicon->setVisible(true);
  //this->setWindowFlags(Qt::SubWindow);
  //this->setWindowFlags((Qt::WindowCloseButtonHint));
  //this->setWindowFlags((Qt::WindowMinMaxButtonsHint));
  //app.setQuitOnLastWindowClosed(true);
  this->hide();
}

void MainWindow::menuDocumentation()
{
}

void MainWindow::menuAbout()
{
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
  this->show();
}

void MainWindow::menuNewPlaylist()
{
  PLSmartNew diag(&util, db);
}


void MainWindow::menuNewSmartPlaylist()
{
  PLNormalNew* diag = new PLNormalNew(&util, db);
}


/*void MainWindow::changeEvent ( QEvent *event )
{
  if( event->type() == QEvent::WindowStateChange )
  {
    if( isMinimized() )
    {
      trayicon->setVisible(true);
      this->setVisible(false);
      this->hide();
    }
  }
}*/

