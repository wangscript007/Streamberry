#ifndef TRACKCONTEXT_H
#define TRACKCONTEXT_H
#include <QtGui>

class QMenu;
class QSqlRecord;
class LibraryController;
class Database;

class TrackContext : QMenu
{
  Q_OBJECT

public:
  TrackContext();
  void trackRightClicked(QList<QString> ids, QList<QString> uniqueIDs, LibraryController* passlib, Database* datab);

signals:
  void playthis(QList<QSqlRecord>*);

 public slots:
  void Play();
  void Delete();
  void Hide();

private:
  int setup;
  QList<QString> CurrentIDs;
  QList<QString> CurrentUniqueIDs;
  LibraryController* LibCont;
  Database* db;

};

#endif
