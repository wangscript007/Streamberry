#include "utilities.h"
#include "crossplatform.h"
#include <QDateTime>
#include <QString>
#include <QDebug>


Utilities::Utilities(QString execpath)
{
    execpath.resize(execpath.lastIndexOf('/'));
    execpath += "/";
    stylesheet = makeStylesheet();
}

int Utilities::getCurrentTimestamp()
{
    return QDateTime::fromTime_t(0).secsTo(QDateTime::currentDateTime());
}

QString Utilities::getExecutePath()
{
    return execpath;
}

QString Utilities::makeStylesheet()
{
    //top bar
    QString style = "#topBarBack {background: url(EXECPATHimages/topBarBackUp.png) no-repeat; border: none; padding:0px;}";
    style+= "#topBarBack:pressed {background: url(EXECPATHimages/topBarBackDown.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarForward {background: url(EXECPATHimages/topBarForwardUp.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarForward:pressed {background: url(EXECPATHimages/topBarForwardDown.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarMusic {background: url(EXECPATHimages/topBarMusicVideoUp.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarWidget {background: url(EXECPATHimages/topBarBackground.png); border: none; padding:0px;}";
    style+="#topBarSeparatorRight  {background: url(EXECPATHimages/topBarPlaylistsSeparatorRightAlignLeft.png); border: none; padding:0px;}";
    //bottom bar
    style+="#bottomBarWidget {background: url(EXECPATHimages/botBarBackground.png); border: none; padding:0px;}";
    style+="#bottomBarRepeat {background: url(EXECPATHimages/botBarRepeatAllUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarRepeat:pressed {background: url(EXECPATHimages/botBarRepeatAllDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarShuffle {background: url(EXECPATHimages/botBarShuffleOnUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarShuffle:pressed {background: url(EXECPATHimages/botBarShuffleOnDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarPrevious {background: url(EXECPATHimages/botBarBackUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarPrevious:pressed {background: url(EXECPATHimages/botBarBackDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarNext {background: url(EXECPATHimages/botBarNextUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarNext:pressed {background: url(EXECPATHimages/botBarNextDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarMute {background: url(EXECPATHimages/botBarVolumeOnUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarMute:pressed {background: url(EXECPATHimages/botBarVolumeOnDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarPlaypause {background: url(EXECPATHimages/botBarPlayUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarPlaypause:pressed {background: url(EXECPATHimages/botBarPlayDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarSongInfoLine {background-color: #222222; border: none; padding:0px;}";
    //library
    style+="#libraryTableWidget {background: #1e1e1e; border: none; padding:0px;}";
    style+="#albumArt {background-color: #1e1e1e; border-bottom:1px solid #AAAAAA}";
    //playbutton
    style+="#progressback {background:url(EXECPATHimages/botBarProgress.png) no-repeat; border: none; padding:0px;}";
    style+="#playbutton {background-color:rbg(0,0,0,0); background-repeat:none; background-image:url(EXECPATHimages/botBarPlayUp.png); border: none; padding:0px;}";
    style.replace("EXECPATH", execpath);
    return style;
}

QString Utilities::getStylesheet()
{
    return stylesheet;
}

// function that specifies the colors of the background of the library rows
QString Utilities::getColor(int inputVar)
{
if(inputVar==1)
    return "#222222";

if(inputVar==2)
    return "#2d2d2d";
return "#000000";
}
