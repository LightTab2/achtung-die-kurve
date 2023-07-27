#include "client.h"
#include "mainwindow.h"
/*
Client::Client(MainWindow *parent, Connect* _connect, GameLogic * _logic, GameData *_data, QLabel *_statusLabel, QAction *_actionHost)
: QObject(parent), connection(new QTcpSocket), con(_connect), logic(_logic), data(_data), statusLabel(_statusLabel), actionHost(_actionHost)
{
    QTcpSocket *&socket = connection.socket;
    QTcpSocket::connect(socket, &QTcpSocket::readyRead,     this,       &Client::readData);
    QTcpSocket::connect(socket, &QTcpSocket::connected,     this,       &Client::connected);
    QTcpSocket::connect(socket, &QTcpSocket::errorOccurred, con,        &Connect::errorHandle);

    pinger      .setInterval(static_cast<int>(NetworkSettings::PING_INTERVAL));
    pinger      .callOnTimeout(this, &Client::ping);

    timeout     .setInterval(static_cast<int>(NetworkSettings::TIMEOUT));
    timeout     .setSingleShot(false);
    timeout     .callOnTimeout(this, &Client::timedOut);

    inputSender .setSingleShot(false);
    inputSender .callOnTimeout(this, &Client::sendInput);
}

void Client::connect(std::string name, QHostAddress address, quint16 port)
{
    connection.playerName = name;
    connection.connect(address, port);
}

void Client::connected()
{
    connection.bConnected = true;
    con->connected();

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << connection.token;
    stream.writeBytes(connection.playerName.c_str(), connection.playerName.size());
    connection.sendData(MessageType::name, msg);
}

void Client::ping()
{
    if (connection.bConnected)
        connection.sendData(MessageType::ping);
}

void Client::timedOut()
{
    close();

    QMessageBox::critical(static_cast<QWidget *>(this->parent()), tr("Error"),
                          tr("Connection timeout"));
}

void Client::close()
{
    logic->stopGame();
    timeout.stop();
    pinger.stop();

    actionHost->setDisabled(false);
    statusLabel->setStyleSheet("QLabel { color : rgb(30,30,30); }");
    statusLabel->setText("Not connected");

    connection.socket->close();
    connection.udpSocket->close();
    connection.bConnected = false;
}
*/