#ifndef BEACONSENDER_H
#define BEACONSENDER_H
#include <QThread>
#include <QtNetwork>
#include "database.h"

class beaconsender : public QThread
{
public:
    beaconsender(Database &datab);
    void send();
protected:
    void run();
private:
    Database& db;
    QUdpSocket *udpSocket;
};

#endif // BEACONSENDER_H