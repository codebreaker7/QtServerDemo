#include "dataserver.h"
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>
#include <iostream>

DataServer::DataServer(QObject *parent) : QObject(parent)
{
    // setup network session
    QNetworkConfigurationManager configManager;
    if (configManager.capabilities() && QNetworkConfigurationManager::NetworkSessionRequired) {
        QNetworkConfiguration config = configManager.defaultConfiguration();
        networkSession = new QNetworkSession(config);
        networkSession->open();
        networkSession->waitForOpened(-1);
        connect(networkSession, &QNetworkSession::stateChanged, this, &DataServer::observeSessionState);
    }
    // start server
    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::LocalHost, 33333)) {
        qCritical("Can not start service");
    }
    connect(server, &QTcpServer::newConnection, this, &DataServer::sendData);
}

void DataServer::sendData() {
    QTextStream outs(stdout);
    client = server->nextPendingConnection();
    clients.append(client);
    outs << client->peerAddress().toString() << " " << client->peerPort() << endl;
    outs << clients.size() << " clients connected" << endl;
    outs.flush();
    connect(client, &QIODevice::readyRead, this, &DataServer::receiveData);
    connect(client, &QTcpSocket::disconnected, this, &DataServer::clientDisconnected);
}

void DataServer::receiveData() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    QTextStream outs(stdout);
    // receive request
    QByteArray recdata = client->readAll();
    QString data(recdata);
    outs<<data<<endl;
    outs.flush();
    // process it
    QDir directory(data);
    QByteArray response;
    if (!directory.exists()) {
        response.append(char(0));
        response.append(QString("Not existing directory"));
    } else {
        response.append(char(1));
        QFileInfoList infoList = directory.entryInfoList();
        foreach (QFileInfo fileInfo, infoList) {
            response.append(fileInfo.fileName());
            response.append(';');
        }
    }
    // write response
    client->write(response);
    client->flush();
}

void DataServer::clientDisconnected() {
    QTextStream outs(stdout);
    outs << "Client disconnected" << endl;
    QTcpSocket * client = qobject_cast<QTcpSocket*>(sender());
    clients.removeAll(client);
    outs << clients.size() << " clients connected" << endl;
    outs.flush();
}

void DataServer::observeSessionState(QNetworkSession::State newState) {
    QTextStream outs(stdout);
    switch (newState) {
    case QNetworkSession::Invalid:
        outs << "Session is in invalid state" << endl;
        break;
    case QNetworkSession::NotAvailable:
        outs << "Session is not available" << endl;
        break;
    case QNetworkSession::Connecting:
        outs << "Session is in connecting state" << endl;
        break;
    case QNetworkSession::Connected:
        outs << "Session is in connected state" << endl;
        break;
    case QNetworkSession::Closing:
        outs << "Session is in closing state" << endl;
        break;
    case QNetworkSession::Disconnected:
        outs << "Session is in disconnected state" << endl;
        break;
    case QNetworkSession::Roaming:
        outs << "Session is in roaming state" << endl;
        break;
    default:
        break;
    }
    outs.flush();
}

