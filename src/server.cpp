#include "server.h"
#include "mainwindow.h"
/*
Server::Server(MainWindow *parent, Host *_host, GameLogic *_logic, GameData *_data, QLabel *_statusLabel, QAction* _actionConnect) :
    QObject(parent), server(new QTcpServer(this)), host(_host), logic(_logic), data(_data), statusLabel(_statusLabel), actionConnect(_actionConnect), positionConnection(new QTcpSocket())
{
    connect(&server, &QTcpServer::newConnection, this, &Server::acceptConnection);

    timeout.setSingleShot(false);
    timeout.setInterval(static_cast<int>(NetworkSettings::PING_INTERVAL));
    timeout.callOnTimeout(this, &Server::checkTimeout);
    timeout.start();

    read.setInterval(static_cast<int>(1000.0/GameSettings::TICK_RATE));
    read.callOnTimeout(this,    &Server::readData);
    read.setSingleShot(false);
    read.start();
}

void Server::close()
{
    server.close();
    clients.clear();

    statusLabel->setStyleSheet("QLabel { color : rgb(30,30,30); }");
    statusLabel->setText("Not connected");
    actionConnect->setDisabled(false);

    logic->stopGame();
    data->players.clear();
}

void Server::checkTimeout()
{
    for (ManagedConnection &client : clients)
        if (client.lastReceived.elapsed() > static_cast<int>(NetworkSettings::TIMEOUT))
        {
            terminateConnection(&client);
            //QMessageBox::critical(static_cast<QWidget *>(this->parent()), tr("Server"),
            //                      tr("Timeout!"));
        }
}

bool Server::listen(QHostAddress address, quint16 port)
{
    if (!server.listen(address, port))
    {
        QMessageBox::critical(static_cast<QWidget *>(this->parent()), tr("Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server.errorString()));
        return false;
    }
    positionConnection.address = address.toIPv4Address();
    positionConnection.bindUdpSocket(port);
    return true;
}
*/