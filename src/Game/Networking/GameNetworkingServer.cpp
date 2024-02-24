/*
void Server::acceptConnection()
{
    while (server.hasPendingConnections())
    {
        clients.emplace_back(server.nextPendingConnection());
        ManagedConnection& connection = clients.back();
        connection.bConnected = true;
        //Unikalny port
        for (unsigned short freePort = NetworkSettings::UDP_PORT_START; freePort != NetworkSettings::UDP_PORT_START + static_cast<unsigned short>(clients.size()) + 1u; ++freePort)
        {
            bool bFree = true;
            for (ManagedConnection &client : clients)
            {
                if (freePort == client.udpPort)
                {
                    bFree = false;
                    break;
                }
            }
            if (bFree)
            {
                connection.udpPort = freePort;
                break;
            }
        }
    }
}

//TCP
void Server::readData()
{
    //Klient też wywołuje tą funkcję, ale tablica clients jest pusta, więc nic nie jest zrobione
    for (int i = 0; i != static_cast<int>(clients.size()); ++i)
    {
        ManagedConnection *connection = &clients.at(i);
        if (connection->bConnected)
        {
            QByteArray msg;
            unsigned char messType;
            while (connection->readData(messType, msg))
            {
                QDataStream stream(&msg, QIODevice::ReadOnly);
                if (messType == MessageType::ping)
                    connection->sendData(MessageType::ping);
                else if (messType == MessageType::name)
                {
                    quint32 token = 0u;
                    stream >> token;
                    char *name;
                    unsigned size;
                    stream.readBytes(name, size);

                    Player* player = data->findPlayer(name);
                    connection->playerName = name;

                    QByteArray msgSend;
                    QDataStream sendStream(&msgSend, QIODevice::WriteOnly);
                    if (player)
                    {
                        if (player->token != token)
                        {
                            sendStream.writeBytes("\x01", 2u);
                            connection->sendData(MessageType::name, msgSend);

                            terminateConnection(connection);
                            --i;
                        }
                        else if (player->bActive)
                        {
                            sendStream.writeBytes("\x04", 2u);
                            sendStream << (connection->token=token) << connection->udpPort;

                            connection->sendData(MessageType::name, msgSend);
                            sendStartGameMessage(*connection);
                        }
                    }
                    else
                    {
                        //Dodanie nowego gracza i połączenie ManagedConnection z nim (synchronizacja nazwy i tokenów)
                        data->players.emplace_back(name);
                        connection->playerName = name;
                        //Unikalny token
                        bool bRollingToken = true;
                        while (bRollingToken)
                        {
                            bRollingToken = false;

                            token = QRandomGenerator::global()->generate();

                            //Być może akurat rozłączony gracz ma taki sam token
                            for (Player &player : data->players)
                                if (token == player.token)
                                {
                                    bRollingToken = true;
                                    break;
                                }
                        }
                        data->players.back().token = connection->token = token;
                        if (data->players.size() < 2)
                        {
                            sendStream.writeBytes("\x02", 2u);
                            connection->sendData(MessageType::name, msgSend);
                        }
                        else
                        {
                            for (ManagedConnection& con : clients)
                            {
                                QByteArray msgS;
                                QDataStream sendS(&msgS, QIODevice::WriteOnly);
                                sendS.writeBytes("\x03", 2u);
                                sendS << con.token << con.udpPort;
                                con.sendData(MessageType::name, msgS);
                            }
                            startGame();
                        }
                    }
                    delete[] name;
                }
                else break;
                connection->lastReceived.start();
                msg.clear();
            }
        }
    }
}

//UDP
void GameLogic::readDatagramsServer()
{
    QByteArray msg;
    unsigned char msgType;

    quint32 sender;
    unsigned short senderPort;
    while (server->positionConnection.readDatagram(msgType, msg, &sender, &senderPort))
    {
        QDataStream stream(&msg, QIODevice::ReadOnly);
        if (msgType == MessageType::resendPos)
        {
            std::vector<ManagedConnection::ResendData> resend;
            while (true)
            {
                QByteArray unit;
                unsigned size;
                char *datagramData;
                if (stream.atEnd())
                    break;
                stream.readBytes(datagramData, size);
                unit.append(datagramData, static_cast<int>(size));

                QDataStream unitStream(&unit, QIODevice::ReadOnly);
                char* playerName;
                unitStream.readBytes(playerName, size);

                Player *player = data->findPlayer(playerName);
                if (!player)
                    continue;
                resend.emplace_back(player);
                while (true)
                {
                    if (unitStream.atEnd())
                        break;

                    unsigned res;
                    unitStream >> res;
                    resend.back().confirm.push_back(res);
                }
                delete[] datagramData;
                delete[] playerName;
            }
            QByteArray resendMsg = buildRepositionMessage(resend);
            server->positionConnection.sendDatagram(MessageType::sendPos, resendMsg, sender, senderPort);
        }
        else if (msgType == MessageType::sendInput)
        {
            QByteArray unit;
            unsigned size;

            char *playerName;
            stream.readBytes(playerName, size);
            Player * player = data->findPlayer(playerName);
            if (player)
                stream >> player->input;
            delete[] playerName;
        }
        msg.clear();
    }
}

void Server::startGame()
{
    if (data->bRunning)
        return;

    statusLabel->setStyleSheet("QLabel { color : rgb(30,30,30); }");
    statusLabel->setText("Waiting for players");

    if (!clients.size())
        return;

    qint64 tickStart = QDateTime::currentMSecsSinceEpoch();

    //Jeżeli uda się odpalić grę, to wysyła odpowiednie wiadomości do wszystkich połączonych klientów
    if (logic->startGame(tickStart))
        for (ManagedConnection &client : clients)
            sendStartGameMessage(client);
}

void Server::sendStartGameMessage(ManagedConnection &client)
{
    //Gracz może się pechowo połączyć w trakcie startowania gry, więc nie będziemy kazali mu oglądać gry, w której nie uczestniczy.
    //W sumie tylko dlatego, że napisaliśmy w opisie projektu, że w tej sytuacji dostanie wiadomość, że musi poczekać na następną rundę, bo oglądanie gry i czekanie na nową rundę brzmi lepiej
    //Istnieje chyba wyjście z tej sytuacji - połączenie obu. Oglądanie gry z napisem na górze, że trzeba poczekać na nową rundę
    if (!(client.getPlayer(data->players)))
        return;
    QByteArray  msg, ticks;
    QDataStream stream(&msg, QIODevice::WriteOnly),
        ticksStream(&ticks, QIODevice::WriteOnly);

    stream << data->players.size();
    for (Player& player : data->players)
    {
        stream << player.color << player.bDead;
        stream.writeBytes(player.name.c_str(), static_cast<uint>(player.name.size()));
    }
    ticksStream << logic->startTime << logic->endTime << logic->moveIndex.data << logic->moveIndex.compare << data->bWon;
    ticksStream.writeBytes(data->winner.c_str(), data->winner.size());

    QByteArray ba;
    QBuffer buff(&ba);
    buff.open(QIODevice::WriteOnly);
    data->boardLocal.save(&buff, "BMP");
    ticksStream.writeBytes(ba, static_cast<uint>(ba.size()));
    GameSettings::sendGameSettings(&client);

    client.sendData(MessageType::playerSync, msg);
    client.sendData(MessageType::tickSync, ticks);
}

void GameLogic::sendPosition()
{
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << moveIndex; //obecny tick
    //Tworzymy wiadomość z pozycją każdego gracza w nowym ticku
    for (Player& player : data->players)
    {
        QByteArray playerData;
        QDataStream streamPlayer(&playerData, QIODevice::WriteOnly);
        if (!player.bActive)
            continue;

        streamPlayer.writeBytes(player.name.c_str(), static_cast<uint>(player.name.size()));
        streamPlayer    << static_cast<unsigned>(moveIndex)
                        << player.data[static_cast<unsigned>(moveIndex)];

        stream      .writeBytes(playerData, static_cast<uint>(playerData.size()));
        playerData.clear();
    }

    //Wysyłanie do każdego klienta
    for (ManagedConnection& con : server->clients)
        con.sendDatagram(MessageType::sendPos, msg);
}

QByteArray GameLogic::buildRepositionMessage(std::vector<ManagedConnection::ResendData>& resend)
{
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << moveIndex; //obecny tick
    for (ManagedConnection::ResendData& res : resend)
    {
        QByteArray  playerMsg;
        QDataStream playerStream(&playerMsg, QIODevice::WriteOnly);
        Player      *&player = res.player;

        if (!player->bActive)
            continue;

        playerStream.writeBytes(player->name.c_str(), static_cast<uint>(player->name.size()));

        //Wysyłamy każdą pozycję o indeksie znajdującym się w tablicy [res.confirm], która została
        //zbudowana na podstawie tablicy [player.confirm] u klienta i przesłana do serwera
        for (unsigned i : res.confirm)
            playerStream << i << player->data[i];

        stream.writeBytes(playerMsg, static_cast<unsigned>(playerMsg.size()));
    }

    return msg;
}

void Server::terminateConnection(ManagedConnection *connection)
{
    connection->close();

    auto connection_it = std::find_if(clients.begin(), clients.end(), [&connection](ManagedConnection& p) { return p.socket == connection->socket; });
    if (connection_it != clients.end())
        clients.erase(connection_it);
}
*/