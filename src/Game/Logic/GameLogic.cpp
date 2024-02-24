#include "Game/Logic/GameLogic.h"

#include <QPainter>
#include <QRandomGenerator>

#include <iostream>

namespace Game
{
    Logic::Logic(gsl::not_null<Settings*> gameSettings__)
        : gameSettings_(gameSettings__)
    {
    }

    bool Logic::startGame()
    {
        if (bRunning_)
            return false;

        // Wait up to one second for the tick thread to finish
        if (tickThread_ && !tickThread_->isFinished())
            tickThread_->wait(1000);
        // Since Qt 6.3 it will interrupt the thread and force it to quit, if it didn't finish already
        tickThread_.reset(QThread::create(&Logic::tick, this));
        bRunning_ = true;

        //if ((bServer = (server->clients.size() != 0)))
        //    for (ManagedConnection& con : server->clients)
        //        con.reset();

        // false remove
        if (false && players_.size() < 2)
        {
            statusColor_ = QColor(30, 30, 30);
            status_ = QStringLiteral("Waiting for players");

            /*if (bServer)
                for (ManagedConnection& con : server->clients)
                {
                    QByteArray msgSend;
                    QDataStream sendStream(&msgSend, QIODevice::WriteOnly);
                    sendStream.writeBytes("\x02", 2u);
                    con.sendData(MessageType::name, msgSend);
                }*/

            return false;
        }

        int boardSize = gameSettings_->getBoardSize();
        emit applySettings(boardSize);
        
        board_ = QImage(boardSize, boardSize, QImage::Format_RGB888);
        board_.fill(Qt::black);
        QPainter boardPainter(&board_);

        boardPainter.setPen(QPen(Qt::cyan, 2 * gameSettings_->wallThickness));
        boardPainter.drawRect(QRect(QPoint(0, 0), QSize(boardSize, boardSize)));

        boardSafety_ = board_.copy();

        boardCollision_ = QImage(boardSize, boardSize, QImage::Format_RGB888);
        boardCollision_.fill(Qt::black);

#ifdef _DEBUG
        players_.resize(8);
        players_[0].bLocal = true;
        players_[1].bLocal = true;
        players_[1].left  = Qt::Key_Left;
        players_[1].right = Qt::Key_Right;
#endif

        for (gsl::index i = 0; i != players_.size(); ++i)
        {
            Player& player = players_[i];
            player.reset(gameSettings_);
            player.setColorIndex(i);
        }
        
        bWon_ = false;
        winner_ = "";
        tick_ = 0;

        int maxIndex = gameSettings_->getTimeoutTicks() - 1; // Maximum ticks of desync between confirmIndex_ and currentIndex_. Timeout after that
        Ensures(maxIndex > 0);
        currentIndex_ = WrappingData<int>{0, maxIndex};
        confirmIndex_ = WrappingData<int>{0, maxIndex};

        tickInterval_ = 1000.0 / gameSettings_->getTickRate();
        Ensures(tickInterval_ > 1.0);

        gameStartETimer_.start();
        tickThread_->start(QThread::HighPriority);
    //Obsłużenie spóźnienia się z wystartowaniem gry na kliencie
    //Teraz algorytm retransmisji robi to lepiej
    /*if (!bServer && elapsed >= gameSettings_->FREEZE_TIME)
    {
        ticksOverhead -= gameSettings_->FREEZE_TIME / gameSettings_->TICK_RATE;
        if (ticksOverhead >= gameSettings_->DATA_SEND_TIMEOUT) //jeżeli użytkownik nie namieszał w plikach konfiguracyjnych, to nigdy nie powinno się wydarzyć
        {
            client->close();
            return false;
        }
        //Nie powinno się zdarzyć i może wywołać problemy graficzne, jeżeli jakimś cudem serwer odpali grę później, niż startTime
        //Teoretycznie powinniśmy to obsłużyć, ale jest to bardzo trudne
        //Trzeba by wysłać pakiet z poprawionym czasem albo poprawić błędy w rysowaniu wynikające z tego
        moveIndex = static_cast<unsigned short>(ticksOverhead);

        for (Player& player : data->players)
            for (unsigned i = 1; i <= static_cast<unsigned>(moveIndex); ++moveIndex)
            {
                player.data[i].timestamp = startTime + static_cast<qint64>(gameSettings_->FREEZE_TIME);
                player.data[i].noDraw = true;
            }
    }*/
    /*
        if (bServer)
            for (Player& player : data->players)
            {
                player.data[0].timestamp = startTime + static_cast<qint64>(gameSettings_->FREEZE_TIME);
                player.pos_x = player.data[0].pos_x = QRandomGenerator::global()->bounded(gameSettings_->wallThickness + (gameSettings_->PLAYER_SIZE << 1), gameSettings_->BOARD_SIZE - gameSettings_->wallThickness - (gameSettings_->PLAYER_SIZE << 1));
                player.pos_y = player.data[0].pos_y = QRandomGenerator::global()->bounded(gameSettings_->wallThickness + (gameSettings_->PLAYER_SIZE << 1), gameSettings_->BOARD_SIZE - gameSettings_->wallThickness - (gameSettings_->PLAYER_SIZE << 1));
                player.angle = player.data[0].angle = QRandomGenerator::global()->bounded(0, static_cast<int>(200*M_PI))/100.0;
            }
        else if (QDateTime::currentMSecsSinceEpoch() < startTime + static_cast<qint64>(gameSettings_->FREEZE_TIME))
        {
            //Będzie pytać o retransmijsę pozycji i uzyskamy w ten sposób pierwszą pozycję gracza
            for (Player& player : data->players)
            {
                player.data[0].timestamp = startTime + static_cast<qint64>(gameSettings_->FREEZE_TIME);
                player.confirm[0] = player.confirm[1] = false;
                player.data[0].noDraw = true;
            }
        }
        client->inputSender.setInterval(static_cast<int>(1000.0/static_cast<double>(gameSettings_->TICK_RATE)));
        client->inputSender.start();

        return true;
        */
    }

    void Game::Logic::tick()
    {
        if (!bRunning_)
            return;
        /*
        if (bServer)
        {
            readDatagramsServer();
        }
        else
        {
            readDatagramsClient();
            askForResendPosition();
        }
        */

        int freezeTicks = gameSettings_->getFreezeTicks();
        if (tick_ >= freezeTicks)
        {
            if (bWon_ && tick_ >= gameSettings_->getWinWaitTicks())
            {
                //stopGame();
                //if (bServer) server->startGame(); //restart
                //return;
            }

            WrappingData<int> nextIndex = currentIndex_ + 1;

            for (Player& player : players_)
                player.calculateNextMove(nextIndex, gameSettings_->getSpeedPerTick(), gameSettings_->getRotationSpeedPerTick());

            if (tick_ >= freezeTicks + gameSettings_->getGraceTicks())
            {
                for (Player& player : players_)
                {
                    rollNoDraw(player);
                    player.checkWallCollision(nextIndex, gameSettings_->getBoardSize(), gameSettings_->wallThickness);
                }
                if (tick_ > gameSettings_->getSafetyTicks())
                    checkPlayerCollision(nextIndex);

                updateMap();
            }

            ++currentIndex_;

            //if (bServer)
            //else
        }
        ++tick_;
        {
            static int tickrate = 0;
            static QElapsedTimer elapsedT;
            if (!elapsedT.isValid())
                elapsedT.start();
            ++tickrate;
            qint64 elap = elapsedT.elapsed();
            if (elap > 1000)
            {
                if (qRound((elap - 1000) / tickInterval_) > 0)
                    --tickrate;
                elapsedT.start();
                ::std::cout << "tickrate/s: " << tickrate << ::std::endl;
                tickrate = 0;
            }
        }
        qint64 elapsed = gameStartETimer_.elapsed();
        nextTick_ += tickInterval_;
        //catch-up
        if (elapsed > nextTick_)
            nextTick_ = elapsed - std::fmod(elapsed, tickInterval_) + tickInterval_;
        QTimer::singleShot(qMax(0, qRound(nextTick_ - elapsed)), this, &Logic::tick);
    }

    //void Game::Logic::tick()
    //{
        /*
        //Oblicz nowy ruch dla każdego gracza
        for (Player& player : data->players)
            if (player.bActive)
            {
                Player::MovementData& newData = player.data[static_cast<unsigned>(moveIndex + 1)];
                newData.timestamp = QDateTime::currentMSecsSinceEpoch();
                newData.input = player.input;
                player.calculateNextMove(moveIndex, (QDateTime::currentMSecsSinceEpoch() - startTime < static_cast<qint64>(gameSettings_->FREEZE_TIME) + static_cast<qint64>(gameSettings_->GRACE_TIME)));
                //Do wyświetlania na serwerze (serwer też ma okienko!)
                player.pos_x = newData.pos_x;
                player.pos_y = newData.pos_y;
                player.angle = newData.angle;
            }

        if (elapsed >= static_cast<qint64>(gameSettings_->FREEZE_TIME) + static_cast<qint64>(gameSettings_->GRACE_TIME))
        {
            unsigned index = 0;
            //Kolizja
            for (Player& player : data->players)
            {
                player.checkWallCollision(moveIndex);
                drawPlayerLine(player, index, true);
                ++index;
            }
            //if (!(moveIndex % gameSettings_->SMOOTH_INTERVAL))
            checkPlayerCollision(moveIndex);
            //Rysowanie
            for (Player& player : data->players)
            {
                drawPlayerLine(player);
                rollNoDraw(player);
            }
        }

        ++drawIndex;

        sendPosition();
        if (!data->bWon) checkWin();
        ++moveIndex;*/
    //}

    void Game::Logic::checkWin()
    {
        /*
        if (data->bWon)
            return;

        data->bWon = true;
        data->winner = "";

        for (Player &player : data->players)
            if (player.bActive && !player.bDead)
            {
                if (data->winner == "")
                {
                    data->winner = player.name;
                    continue;
                }
                data->bWon = false;
                break;
            }

        if (data->bWon)
        {
            statusLabel->setStyleSheet("QLabel { color : rgb(180,180,20); }");
            if (data->winner != "")
                statusLabel->setText(("The winner is " + data->winner + '!').c_str());
            else
                statusLabel->setText("A draw!");

            endTime = QDateTime::currentMSecsSinceEpoch();

            for (ManagedConnection &con : server->clients)
            {
                if (!con.bConnected)
                    continue;
                Player *localPlayer = con.getPlayer(data->players);
                if (!localPlayer || !localPlayer->bActive)
                    continue;

                QByteArray msg;
                QDataStream stream(&msg, QIODevice::WriteOnly);
                stream << endTime;
                if (data->winner != "")
                    stream.writeBytes(data->winner.c_str(), static_cast<uint>(data->winner.size()));
                con.sendData(MessageType::gameEnd, msg);
            }
        }*/
    }

    void Game::Logic::stopGame() //Kończy grę
    {
        /*
        tickTimer.stop();
        data->bRunning = false;
        if (!bServer)
        {
            client->connection.reset();
            client->inputSender.stop();
        }
        else
        {
            for (unsigned i = 0; i != data->players.size();)
            {
                bool toDelete = true;
                for (ManagedConnection& client : server->clients)
                {
                    if (!(data->players[i].name.compare(client.playerName)))
                    {
                        toDelete = false;
                        break;
                    }
                }
                if (toDelete)
                    data->players.erase(data->players.begin() + static_cast<int>(i));
                else ++i;
            }
        }*/
    }

    void Logic::localPlayerKeyPressed(Qt::Key key)
    {
        for (Player& player : players_)
        {
            if (!player.bLocal)
                continue;
            if (key == player.left)
                player.startRotating(Player::Direction::left);
            else if (key == player.right)
                player.startRotating(Player::Direction::right);
        }
    }

    void Logic::localPlayerKeyReleased(Qt::Key key)
    {
        for (Player& player : players_)
        {
            if (!player.bLocal)
                continue;
            if (key == player.left)
                player.stopRotating(Player::Direction::left);
            else if (key == player.right)
                player.stopRotating(Player::Direction::right);
        }
    }

    const QImage* Logic::getBoardImage() const
    {
        return &board_;
    }

    void Game::Logic::endGame() //Ogłasza zwyciężcę i odlicza czas do zakończenia gry
    {
        /*
        this->endTime = _endTime;
        data->bWon = true;

        statusLabel->setStyleSheet("QLabel { color : rgb(180,180,20); }");
        if (data->winner != "")
            statusLabel->setText(("The winner is " + data->winner + '!').c_str());
        else
            statusLabel->setText("A draw!");*/
    }

    void Game::Logic::checkPlayerCollision(const WrappingData<int>& index)
    {
        const unsigned char *bytes          = board_.constBits();
        const unsigned char *bytesCollision = boardCollision_.constBits();
        const int wallThicknessPixels = 3 * gameSettings_->wallThickness;
        const int boardSizeLinePixels = 3 * gameSettings_->getBoardSize() - 2 * wallThicknessPixels; // Area - 2 walls = drawableArea. This is single line or the drawable area
        const int boardSizeFullPixels = (3 * gameSettings_->getBoardSize() * gameSettings_->getBoardSize()) - wallThicknessPixels; // Everything minus right wall = end of drawing area

        for (int i_r = wallThicknessPixels; i_r < boardSizeFullPixels; i_r += wallThicknessPixels)
        {
            for (int endOfLine = i_r + boardSizeLinePixels; i_r != endOfLine; i_r += 3)
            {
                if (bytesCollision[i_r] == 255u && bytes[i_r] != 0u)
                {
                    const int i_g = i_r + 1;
                    const int i_b = i_r + 2;
                    int deadIndex  = bytesCollision[i_b];
                    Player& killed = players_[deadIndex];
                    if ( (killed.getColor() == QColor(bytes[i_r], bytes[i_g], bytes[i_b])) && (boardSafety_.constBits()[i_r] == 0u) )
                        continue;
                    killed.die(index);
                }
            }
        }
    }

    void Game::Logic::rollNoDraw(Player& player)
    {
        if (QRandomGenerator::global()->generateDouble() <= gameSettings_->getGapChancePerTick())
            player.setNoDrawTicks(QRandomGenerator::global()->bounded(gameSettings_->getGapMinTicks(), gameSettings_->getGapMaxTicks()));

    }

    /*void Client::keyPressed(int key)
    {
        Player* player = connection.getPlayer(data->players);
        if (!player)
            return;
        switch (key)
        {
        case Qt::Key_A:
            player->startRotating(true);
            break;
        case Qt::Key_D:
            player->startRotating(false);
            break;
        }
    }

    void Client::keyReleased(int key)
    {
        Player* player = connection.getPlayer(data->players);
        if (!player)
            return;
        switch (key)
        {
        case Qt::Key_A:
            player->stopRotating(true);
            break;
        case Qt::Key_D:
            player->stopRotating(false);
            break;
        }
    }
    */
/*
    Player* Game::Logic::findPlayer(const QString& playerName) const
    {
        auto it = std::find(players_.begin(), players_.end(), playerName);

        if (it == players.end())
            return nullptr;

        return &*it;
    }*/
}