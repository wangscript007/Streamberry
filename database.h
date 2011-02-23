#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QString>
#include <QStringList>
#include <QSqlRecord>
#include <QList>

using namespace std;

class Database: public QObject
{
public:
    Database();
    virtual ~Database();
    //initialises the database
    void initialise();
    //updates or adds setting to database. please do not make typos in parameters, it would mess up the db.
    void storeSetting(QString name, QString value);
    //returns value of setting given to name parameter. returns NULL if not found.
    QString getSetting(QString name);
    //set list of folders for file scanner to search through. takes semi-colon separated string.
    //note: clears table first, so any folders not in this list will be removed!
    void setFolders(QString folders);
    //takes QStringList instead of string
    void setFolders(QStringList folderlist);
    //get list of folders for file scanner to search through. returns QStringList containing folders.

    QStringList getFolders();
    //gets the last update of the library of a specified user
    QString lastUpdate(QString user_name);
    //get all changes from Local after timestamp. takes this computer's unique ID.
    QString changesSinceTime(int timestamp, QString uniqueID);
    //set a user online or offline
    void setOnline(QString uniqueID, QString status);
    //add new changes
    void setNewChanges(QString changes);
    void makeUser(QString timeLastUpdated, QString timeLastOnline, QString uniqueID, QString name);

    //if argument 0 gets tracked, if 1 gets excluded
    QStringList getFolders(int trackedOrExcluded);
    //counts the number of rows in a table
    int rowCount(QString tablename);
    //adds file to database. if already in database it just updates.
    void addFile(QString filepath, QString filename, QString filesize, QString artist, QString album, QString title, QString genre, QString rating, QString year, QString length, QString bitrate, QString filetype, QString table, QString UniqueID);
    //deletes file by ID. returns number of files deleted.
    int deleteFile(QString id, QString table);
    //perform search.
    //type: 0 - all, 1 - artist, 2 - title, 3 - genre, 4 - album
    //return order: Filepath, Artist, Album , Title , Genre, Rating , Filename , Year , Length , Bitrate , Filesize , Timestamp , Filetype
    QList<QSqlRecord>* searchDb(int type, QString searchtxt, QList<QString>& sortcols, QList<QString> order);

    QSqlQuery query(QString sql); //Shouldn't this be private?

    //Returns the UniqueID of the local machine for the use of Filescan
    QString getUniqueID();
    //Only for testing purposes: Sets the unique ID of the local machine to a certain value.
    void setUniqueID(QString id);
    //This takes a playlist name and returns a QSqlQuery containing information about the playlists type and filter.
    QSqlQuery GetPlaylistInfo(QString playlistName);
    //This takes a playlist name and returns an QSqlQuery containing the IDs and Unique IDs of all the tracks in the playlist
    QSqlQuery GetPlaylistTracks(QString playlistName);
    //Takes a QList of track unique IDs and IDs, and returns a QList of QSqlRecords containing all the Track data from across all tables.
    QList<QSqlRecord> getTracks( QList<QString> Tracks);
    //Takes a playlist name, smart bool and a filter and enters it into the playlist table.
    void PlaylistSave(QString name, int smart, QString filter);
    //Takes a list of tracks and a playlist name and enters them into the playlistTracks table
    void PlaylistAddTracks(QList<QString> Tracks, QString Playlist);
    //Deletes a playlist from the playlist table and it's tracks from the playlistTracks table
    void removePlaylist(QString name);

  private:
    //connects to database in given path
    void connect(QString &path);
    //creates database file
    void createDatabase(QString &path);
    //performs an SQL query


    QSqlDatabase db;
    //says whether database is connected or not
    bool connected;
    //file name not including path
    QString dbfilename;
};

#endif // DATABASE_H
