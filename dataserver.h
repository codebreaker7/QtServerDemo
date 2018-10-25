#ifndef DATASERVER_H
#define DATASERVER_H

#include <QObject>
#include <QtNetwork>
#include <QList>

class DataServer : public QObject
{
    Q_OBJECT
public:
    explicit DataServer(QObject *parent = 0);
signals:

public slots:
    void sendData();
    void receiveData();
    void clientDisconnected();
    void observeSessionState(QNetworkSession::State);
private:
    QTcpServer * server;
    QList<QTcpSocket *> clients;
    QTcpSocket * client;
    QNetworkSession * networkSession;
};

#endif // DATASERVER_H
