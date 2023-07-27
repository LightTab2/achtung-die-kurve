#include "managedconnection.h"
/*
void GameSettings::sendGameSettings(ManagedConnection *connection)
{
    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);

    stream << BOARD_SIZE;
    stream << WALL_THICKNESS;
    stream << PLAYER_SIZE;
    stream << LINE_SIZE;
    stream << NO_DRAW_MIN;
    stream << NO_DRAW_MAX;
    stream << COLLISION_DRAW_FIRST_IGNORE;
    stream << DATA_SEND_TIMEOUT;
    stream << WIN_TIME;
    stream << GRACE_TIME;
    stream << FREEZE_TIME;
    stream << TICK_RATE;
    stream << FREEZE_FONT_SIZE;
    stream << WIN_FONT_SIZE;
    stream << NAME_FONT_SIZE;

    stream << SPEED;
    stream << ANGLE_SPEED;
    stream << NO_DRAW_CHANCE;

    connection->sendData(MessageType::gameStart, arr);
}

void GameSettings::loadGameSettings(QByteArray arr)
{
    QDataStream stream(&arr, QIODevice::ReadOnly);

    stream >> BOARD_SIZE;
    stream >> WALL_THICKNESS;
    stream >> PLAYER_SIZE;
    stream >> LINE_SIZE;
    stream >> NO_DRAW_MIN;
    stream >> NO_DRAW_MAX;
    stream >> COLLISION_DRAW_FIRST_IGNORE;
    stream >> DATA_SEND_TIMEOUT;
    stream >> WIN_TIME;
    stream >> GRACE_TIME;
    stream >> FREEZE_TIME;
    stream >> TICK_RATE;
    stream >> FREEZE_FONT_SIZE;
    stream >> WIN_FONT_SIZE;
    stream >> NAME_FONT_SIZE;

    stream >> SPEED;
    stream >> ANGLE_SPEED;
    stream >> NO_DRAW_CHANCE;
}

void ManagedConnection::connect(QHostAddress _address, unsigned short _port)
{
    close();
    
    address = _address.toIPv4Address();
    udpPort = port = _port;

    socket->connectToHost(_address, port);
}

bool ManagedConnection::bindUdpSocket(unsigned short _port)
{
    udpPort = _port;
    bool binded = udpSocket->bind(QHostAddress(address), udpPort, QAbstractSocket::ShareAddress);
    
    return binded;
}

void ManagedConnection::sendData(const unsigned char msgType, QByteArray msg)
{
    if (socket->state() != QTcpSocket::ConnectedState)
        return;

    QByteArray sendMsg;
    QDataStream stream(&sendMsg, QIODevice::WriteOnly);
    
    stream << msgType;
    stream.writeBytes(msg, static_cast<unsigned>(msg.size()));
    
    socket->write(sendMsg);
}

void ManagedConnection::sendDatagram(const unsigned char msgType, QByteArray msg, unsigned short destPort)
{
    sendDatagram(msgType, msg, 0, destPort);
}

void ManagedConnection::sendDatagram(const unsigned char msgType, QByteArray msg, quint32 destAddr, unsigned short destPort)
{
    if (!destPort)
        destPort = udpPort;
    
    if (!destAddr)
        destAddr = address;
    
    QByteArray  sendMsg;
    QDataStream stream(&sendMsg, QIODevice::WriteOnly);
    
    stream  << msgType;
    stream.writeBytes(msg, static_cast<unsigned>(msg.size()));

    QHostAddress addr(QHostAddress::Any);   //Dowolny interfejs sieciowy
    addr.setAddress(destAddr);
    
    udpSocket->writeDatagram(sendMsg, addr, destPort);
}

bool ManagedConnection::readData(unsigned char& msgType, QByteArray& msg)
{
    if (socket->state() != QTcpSocket::ConnectedState)
        return false;

    //Przetwarzanie uzwględnia fakt, że wiadomość może dotrzeć niepełna, dlatego wykorzystujemy bufor [data]
    data    += socket->readAll();

    //Sprawdzamy, czy wiadomość ma wystarczająca długość, żeby zawierać:
    //- typ wiadomości (unsigned char)
    //- rozmiar wiadomości (qint32)
    if (static_cast<unsigned>(data.size()) < sizeof(unsigned char) + sizeof(qint32))
        return false;

    msgType     = static_cast<unsigned char>(data.at(0));

    QByteArray  sizeMsg = data.left(sizeof(qint32) + sizeof(unsigned char)).remove(0, sizeof(unsigned char));
    QDataStream sizeStream(&sizeMsg, QIODevice::ReadOnly);

    qint32          size;
    sizeStream  >>  size;

    //Mamy preambułę wiadomości, więc trzeba teraz sprawdzić, czy cała do nas dotarła
    if (static_cast<unsigned>(data.size()) < sizeof(unsigned char) + sizeof(qint32) + static_cast<unsigned>(size))
        return false;

    //Usuwa preambułę z bufora
    data.remove(0, sizeof(unsigned char) + sizeof(qint32));
    //Zapisuje dane w przekazanej przez referencję zmiennej
    msg = data.left(size);
    //Usuwa dane z bufora
    data.remove(0, size);

    return true;
}

bool ManagedConnection::readDatagram(unsigned char& msgType, QByteArray& msg, quint32* sender, unsigned short* senderPort)
{
    if (udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram    datagram    = udpSocket->receiveDatagram();

        QByteArray          readMsg     = datagram.data();
        QDataStream         stream(&readMsg, QIODevice::ReadOnly);
        unsigned            size;

        stream >> msgType;

        char* msgData;
        stream.readBytes(msgData, size);
        msg.append(msgData, static_cast<int>(size));
        delete[] msgData;

        if (sender)
            *sender     = datagram.senderAddress().toIPv4Address();
        if (senderPort)
            *senderPort = static_cast<unsigned short>(datagram.senderPort());
        return true;
    }
    return false;
}

void ManagedConnection::reset()
{
    data.clear();

    while (udpSocket->hasPendingDatagrams())
        udpSocket->receiveDatagram();
}

void ManagedConnection::close()
{
    data.clear();

    if (socket)                         socket->close();
    bConnected = false;
}

Player* ManagedConnection::getPlayer(std::vector<Player>& players)
{
    if (!bConnected || playerName.empty())
        return nullptr;

    auto it = std::find(players.begin(), players.end(), playerName);

    if (it == players.end())
        return nullptr;

    return &*it;
}
*/