/*
//TCP
void Client::readData()
{
    QByteArray msg;
    unsigned char msgType;

    while (connection.readData(msgType, msg))
    {
        QDataStream stream(&msg, QIODevice::ReadOnly);
        if (msgType == MessageType::name)
        {
            char            *msgData;
            unsigned short  port;
            unsigned        size;

            stream.readBytes(msgData, size);

            if (!strcmp(msgData, "\x01")) //nazwa w użyciu
            {
                logic->stopGame();
                close();
                QMessageBox::critical(static_cast<QWidget*>(this->parent()), tr("Error"),
                    tr("Name already in use!"));
            }
            else
            {
                if (data->bRunning)
                    return;

                statusLabel->setStyleSheet("QLabel { color : rgb(30,30,30); }");
                if (!strcmp(msgData, "\x02")) //za mało graczy
                {
                    statusLabel->setText("Waiting for players");

                }
                else //czekamy na nową rundę
                {
                    if (!strcmp(msgData, "\x03"))
                    {
                        stream >> connection.token >> port; //port do komunikacji UDP
                        statusLabel->setText("Waiting for the round end");
                    }
                    else //reconnect
                    {
                        stream >> connection.token >> port; //port do komunikacji UDP
                        statusLabel->setText("Waiting for the reconnection");
                    }
                    connection.bindUdpSocket(port);
                    timeout.start();
                    pinger.start();
                }
            }
            delete[] msgData;
        }
        else if (msgType == MessageType::gameStart)
        {
            GameSettings::loadGameSettings(msg);
        }
        else if (msgType == MessageType::playerSync)
        {
            data->players.clear();

            unsigned size;
            stream >> size;

            for (unsigned i = 0; i != size; ++i)
            {
                QColor      color;
                bool        bDead;
                char        *name;
                unsigned    len;

                stream >> color >> bDead;
                stream.readBytes(name, len);

                data->players.emplace_back(name);
                data->players.back().color = color;
                data->players.back().bDead = bDead;
                delete[] name;
            }
        }
        //Żeby klienci i serwer wystartowali w tym samym czasie
        //Po dodaniu dopracowaniu algorytmu retransmisji, to nie ma aż tak dużego znaczenia, ale nadal przydaje się
        //Do synchronizacji czasu dla przewidywań
        else if (msgType == MessageType::tickSync)
        {
            qint64      msecsStart, msecsEnd;
            unsigned    moveIndexData;
            int         moveIndexCompare;
            bool        bWon;
            stream  >>  msecsStart >> msecsEnd >> moveIndexData >> moveIndexCompare >> bWon;
            char *winner, *boardData;
            uint size;
            QByteArray board;
            stream.readBytes(winner, size);
            if (!winner)
            {
                winner = new char[1];
                winner[0] = '\0';
            }
            stream.readBytes(boardData, size);
            board.append(boardData, static_cast<int>(size));

            data->bRunning = false;
            logic->startGame(msecsStart, msecsEnd, moveIndexData, moveIndexCompare, bWon, winner, board);
            delete[] winner;
            delete[] boardData;
        }
        else if (msgType == MessageType::gameEnd)
        {
            qint64      msecs;
            stream  >>  msecs;

            char*       winner;
            unsigned    size;
            stream.readBytes(winner, size);

            if (!size)
                data->winner = "";
            else data->winner = winner;

            logic->endGame(msecs);
            delete[] winner;
        }
        else break;
        connection.lastReceived.start();
        msg.clear();
    }
    if (timeout.isActive())
        timeout.start();
}

//UDP
void GameLogic::readDatagramsClient()
{
    QByteArray msg;
    unsigned char msgType;
    while (client->connection.readDatagram(msgType, msg))
    {
        QDataStream stream(&msg, QIODevice::ReadOnly);
        if (msgType == MessageType::sendPos)
        {
            BoundUnsigned tick(moveIndex.boundary);
            stream >> tick;
            if (!(tick < moveIndex))
            {
                //Dostaliśmy nowy tick, więc ustawiamy wszystkie wartości pomiędzy na potencjalne zgubione pakiety
                for (Player &player : data->players)
                {
                    if (player.bDead)
                        continue;
                    for (BoundUnsigned i = moveIndex; static_cast<unsigned>(i) != static_cast<unsigned>(tick + 1u); ++i)
                        player.confirm[static_cast<unsigned>(i)] = false;
                }
            }
            moveIndex = tick;
            //Prztwarzanie pozycji graczy
            while (!stream.atEnd())
            {
                QByteArray unit;
                QDataStream unitStream(&unit, QIODevice::ReadOnly);
                char *datagramData;
                unsigned size;
                stream.readBytes(datagramData, size);
                unit.append(datagramData, static_cast<int>(size));

                char *playerName;
                unitStream.readBytes(playerName, size);
                while (!unitStream.atEnd())
                {
                    unsigned index;
                    unitStream >> index;
                    Player::MovementData mov;
                    unitStream >> mov;

                    Player *player = data->findPlayer(playerName);
                    if (player)
                    {
                        player->data[index] = mov;
                        //Potwierdzamy otrzymane dane
                        player->confirm[index] = true;
                        //Do wyświetlania "głowy" gracza
                        if (static_cast<unsigned>(index) == static_cast<unsigned>(moveIndex))
                        {
                            player->pos_x = mov.pos_x;
                            player->pos_y = mov.pos_y;
                            player->angle = mov.angle;
                        }
                    }
                }
                delete[] playerName;
                delete[] datagramData;
                unit.clear();
            }
        }
        msg.clear();
    }
}

void Client::sendInput()
{
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    Player *player = connection.getPlayer(data->players);
    if (player)
    {
        stream.writeBytes(player->name.c_str(), static_cast<uint>(player->name.size()));
        stream << player->input;
        connection.sendDatagram(MessageType::sendInput, msg, connection.port);
    }
}

void GameLogic::askForResendPosition()
{
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    for (Player &player : data->players)
    {
        if (player.bDead || !player.bActive)
            continue;

        QByteArray resendMsg;
        QDataStream resendStream(&resendMsg, QIODevice::WriteOnly);

        resendStream.writeBytes(player.name.c_str(), static_cast<uint>(player.name.size()));

        //Wrzuca indexy niepotwierdzonych pozycji (zgubione pakiety) do wiadomości, by móc prosić o retransmisję
        for (unsigned i = 0u; i != player.confirm.size(); ++i)
            if (!player.confirm[i])
                resendStream << i;

        stream.writeBytes(resendMsg, static_cast<uint>(resendMsg.size()));
        resendMsg.clear();
    }
    //Wysyła wiadomość do serwera
    client->connection.sendDatagram(MessageType::resendPos, msg, client->connection.port);
}
*/