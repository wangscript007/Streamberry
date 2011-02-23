#include "filescan.h"
#include "database.h"
//#include "libvlc/vlc/libvlc_media.h"
#include <QtDebug>
#include <QtSql>
#include <QString>
#include <QFile>
#include <QDir>
#include "sbexception.h"
#include "crossplatform.h"

using namespace std;

Filescan::Filescan(Database &datab): db(datab)
{
    db = datab;
}
//Connects to the database by being passed the database pointer.
//filescan::filescan(Database &datab)
//{
//    db = datab;
//}

//This method builds a database by scanning the TrackedFolders for media files to add
//Doesn't add files stored in exluded folders
//Returns 1 if successful
int Filescan::build_new()
{
    QStringList TrackedFolders;
    QStringList ExFolders;
    QString homeid;
    try
    {
        TrackedFolders = db.getFolders(0);
        ExFolders = db.getFolders(1);
        homeid = db.getUniqueID();
        //For every folder to be tracked, run the scanFolder method
        for(int i=0; i<TrackedFolders.size(); i++)
        {
            QDir passpath = QDir(TrackedFolders.at(i));
            scanFolder(passpath, ExFolders, homeid);
        }
        qDebug() << "File Scan Completed";
        return 1;
    }
    catch(SBException e)
    {
        throw e;
    }
    return 0;
}

//This method builds a database by scanning the TrackedFolders for media files to add
//Doesn't add files stored in exluded folders
//Returns 1 if successful
//Thsi method differs from the standard build_new because it wipes the current library first.
int Filescan::build_new_clean()
{
    QStringList TrackedFolders;
    QStringList ExFolders;
    QString homeid;
    try
    {
      QString sql = "DELETE FROM sqlite_sequence WHERE name=\"LibLocal\";";
      db.query(sql);
      sql = "DELETE FROM LibLocal;";
      db.query(sql);
      TrackedFolders = db.getFolders(0);
      ExFolders = db.getFolders(1);
      homeid = db.getUniqueID();
      for(int i=0; i<TrackedFolders.size(); i++)
      {
        QDir passpath = QDir(TrackedFolders.at(i));
        scanFolder(passpath, ExFolders, homeid);
      }
      qDebug() << "Clean File Scan Completed";
      return 1;
    }
    catch(SBException e)
    {
        throw e;
    }
    return 0;
}

//This method takes a folder path and an array of folders to exclude.
//It calls addFiles on the current folder, then builds a list of directories currently in this folder.
//scanFolder is called on these directories too, so the directory tree is traversed recursively.
//Returns 1 when complete
int Filescan::scanFolder(QDir path, QStringList expaths, QString homeid)
{
    QFileInfoList folderList;
    if(!expaths.isEmpty())
    {
        for(int i=0; i<expaths.size(); i++)
        {
            if(path==expaths[i])
                return 1;
        }
    }
    addFiles(path, homeid);
    path.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    folderList = path.entryInfoList();

    if(!folderList.isEmpty())
    {
        for(int j=0; j<folderList.size(); j++)
        {
            scanFolder(QDir(folderList.at(j).absoluteFilePath()), expaths, homeid);
        }
    }
    return 1;
}


//This method takes a folder path adds all the media files within it to the database.
//Each file is checked before it is added to see if it is already in the database, if it is then it is skipped.
//If a duplicate file is found, e.g. one matching certain criteria, then it is added to the database with a dup flag set 1
//returns 1 if files are added.
//TODO Deal with album art in some way. Look at meta.c in VLC
int Filescan::addFiles(QDir path, QString homeID)
{
    QFileInfoList fileList;
    path.setFilter(QDir::Files);
    fileList = path.entryInfoList();
    if(!fileList.isEmpty())
    {
        for(int i = 0; i<fileList.size(); i++)
        {
            if(ismedia(fileList.at(i))==1)
            {
                db.addFile(fileList.at(i).absoluteFilePath(), fileList.at(i).fileName(), QString::number(fileList.at(i).size()), (QString)"ARTIST", (QString)"ALBUM", (QString)"TITLE", (QString)"GENRE", (QString)"5", (QString)"1991", (QString)"123", (QString)"2400", fileList.at(i).suffix(), (QString)"LibLocal", homeID);
            }
        }
    }
    return 1;
}

//TO DO: Many more compatible filetypes need adding
//This method takes a filepath and returns 1 if the file in question is a media file
int Filescan::ismedia(QFileInfo file)
{
    QString name = file.suffix();
    if(QString::compare("wav",name)==0 || QString::compare("mp3",name)==0 || QString::compare("wma",name)==0 || QString::compare("ogg",name)==0 || QString::compare("aac",name)==0 || QString::compare("mid",name)==0)
    {
        return 1;
    }
    return 0;
}



/*//This method takes a filepath and returns 1 if the exact file in question is already in the database
int filescan::isalreadyindat(QDir file)
{
    int i=0;
    if( (db.selectQuery("SELECT Filepath FROM HomeTable WHERE  Filepath = %s", file.filePath())).isNull );
        return 0
    return 1;
}

//this method takes a filepath and returns 1 if this file is a duplicate
int filescan::isdup(QDir file)
{
    QString name = ((file.entryInfoList()).takeFirst()).fileName();
    uint filesize = ((file.entryInfoList()).takeFirst()).size();
    char* meta = libvlc_media_get_meta( libvlc_media_t *p_md, libvlc_meta_t e_meta );
        return 1;
    return 0;
}
 */

