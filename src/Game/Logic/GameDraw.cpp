#include "Game/Logic/GameLogic.h"

#include <QPainter>

void Game::Logic::drawTickTransition()
{
}

void Game::Logic::updateMap()
{ 
    //if (++currentIndex_ == confirmIndex_)
        //client->timedOut();
    boardCollision_.fill(Qt::black);
    QPainter boardPainter(&board_);
    QPainter boardSafetyPainter(&boardSafety_);
    QPainter boardCollisionPainter(&boardCollision_);
    //while (true || confirmIndex_ != (currentIndex_ - 1))
    {
        //Sprawdza, czy wszyscy gracze w obecnym ,,ticku'' mają potwierdzone dane
        for (int playerIndex = 0; playerIndex != players_.size(); ++playerIndex)
        {
            const Player& player = players_[playerIndex];
            if (!player) 
                continue;

            player.drawLine(&boardPainter, currentIndex_, player.getColor(), gameSettings_->sizeLine);

            int safetyTicks = gameSettings_->getSafetyTicks();

            if (tick_ > gameSettings_->getFreezeTicks() + safetyTicks)
            {
                player.drawLine(&boardSafetyPainter, currentIndex_ - safetyTicks, player.getColor(), gameSettings_->sizeLine);
                player.drawLine(&boardCollisionPainter, currentIndex_, QColor(255, 0, playerIndex), gameSettings_->sizeLine, Qt::FlatCap);
            }
            //Player::MovementData &processedData = player.data_[static_cast<unsigned>(confirmIndex)];
            /*if (processedData.demise)
            {
                if (!player.bDead)
                {
                    player.bDead = true;
                    player.deadPos = QPointF(processedData.pos_x, processedData.pos_y);
                }
            }*/
        }
    }
  
        //Rysowanie
        /*
        for (Player &player : data->players)
        {
            if (player.bDead)
                continue;
            painterBoard.setPen(QPen(player.getColor(), GameSettings::LINE_SIZE));
            Player::MovementData    &prevData       = player.data[static_cast<unsigned>(confirmIndex - 1)],
                                    &processedData  = player.data[static_cast<unsigned>(confirmIndex)];

            if (processedData.demise)
            {
                if (!player.bDead)
                {
                    player.bDead = true;
                    player.deadPos = QPointF(processedData.pos_x, processedData.pos_y);
                }
                continue;
            }

            if (processedData.noDraw)
                continue;

            QPointF beg = QPointF(prevData.pos_x,         prevData.pos_y),
                    end = QPointF(processedData.pos_x,    processedData.pos_y);

            painterBoard.drawLine(beg, end);
        }/*

        //++confirmIndex_;
    }
    /*
    //Kopia potwierdzonego obrazu do obecnie rysowanego
    data->boardLocal = data->board.copy(data->boardLocal.rect());

    //Rysowanie przewidywanego stanu gry
    QPainter painterLocalBoard(&(data->boardLocal));
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    if (static_cast<unsigned>(confirmIndex) != static_cast<unsigned>(lastComputed))
        lastTimestamp = currentTime;

    qreal step = 0.0;
    unsigned stepIndex = 0;
    for (BoundUnsigned conf = confirmIndex, nextConfirmedIndex = confirmIndex; static_cast<unsigned>(conf) != static_cast<unsigned>(moveIndex + 1u); ++conf)
    {
        for (Player &player : data->players)
        {
            if (player.bDead)
                continue;

            painterLocalBoard.setPen(QPen(player.getColor(), GameSettings::LINE_SIZE));
            Player::MovementData    &prevData       = player.data[static_cast<unsigned>(conf - 1u)];
            Player::MovementData    &processedData  = player.data[static_cast<unsigned>(conf)];

            //Jeżeli natkniemy się na potwierdzone dane, to używamy ich, zamiast naszych przewidywań
            //Sprawdzamy, czy takie dane nie wystapują zawczasu i podczas przetwarzania niepotwierdzonych stosujemy
            //interpolację liniową do danych potwierdzonych, o ile takie istnieją
            if (conf >= nextConfirmedIndex)
            {
                BoundUnsigned i = conf + 1u;
                for (; static_cast<unsigned>(i) != static_cast<unsigned>(moveIndex + 1u); ++i)
                    if (player.confirm[static_cast<unsigned>(i)])
                        break;
                nextConfirmedIndex = i;

                qint64 time = player.data[static_cast<unsigned>(nextConfirmedIndex)].timestamp;
                if (static_cast<unsigned>(nextConfirmedIndex) == static_cast<unsigned>(moveIndex + 1u))
                    time = lastTimestamp;

                step = static_cast<qreal>(time - prevData.timestamp)/static_cast<qreal>(static_cast<unsigned>(nextConfirmedIndex - static_cast<unsigned>(conf)) + 1u);
                stepIndex = static_cast<unsigned>(conf);
            }

            if (!player.confirm[static_cast<unsigned>(conf)])
            {
                //Można zrobić osobny index do zapamiętywania, które
                processedData.timestamp = static_cast<qint64>(prevData.timestamp) + static_cast<qint64>(step);
                processedData.input = prevData.input;
                player.calculateNextMove(conf - 1u, elapsed < static_cast<qint64>(GameSettings::FREEZE_TIME) + static_cast<qint64>(GameSettings::GRACE_TIME));
            }

            if (prevData.noDraw)
                continue;

            QPointF beg = QPointF(prevData.pos_x, prevData.pos_y),
                    end = QPointF(processedData.pos_x, processedData.pos_y);

            painterLocalBoard.drawLine(beg, end);
        }
    }

    if (static_cast<unsigned>(confirmIndex) != static_cast<unsigned>(lastComputed))
    {
        //Alternatywny sposób rysowania
        //for (Player &player : data->players)
           //player.data[static_cast<unsigned>(moveIndex + 1)] = player.data[static_cast<unsigned>(moveIndex)];
        lastComputed = confirmIndex;
    }
    //Ostatni ruch jest zawsze przewidywany (ponieważ inaczej i tak musielibyśmy dostać informację od serwera o nowym ticku gry...
    //a przecież po to jest przewidywanie, by zachować płynność gry, gdy nie mamy komunikacji z serwerem)
    for (Player &player : data->players)
    {
        if (player.bDead)
                continue;

        painterLocalBoard.setPen(QPen(player.getColor(), GameSettings::LINE_SIZE));

        Player::MovementData    &prevData       = player.data[static_cast<unsigned>(moveIndex)];
        Player::MovementData    &processedData  = player.data[static_cast<unsigned>(moveIndex + 1)];
        //Player::MovementData    copy            = prevData;
        processedData.timestamp                 = currentTime;

        processedData.input = prevData.input;
        player.calculateNextMove(moveIndex, elapsed < static_cast<qint64>(GameSettings::FREEZE_TIME) + static_cast<qint64>(GameSettings::GRACE_TIME));

        QPointF beg = QPointF(prevData.pos_x, prevData.pos_y),
                end = QPointF(processedData.pos_x, processedData.pos_y);

        if (!prevData.noDraw)
            painterLocalBoard.drawLine(beg, end);

        //Dane do wyświetlania "głów" graczy
        player.pos_x = processedData.pos_x;
        player.pos_y = processedData.pos_y;
        player.angle = processedData.angle;
    }
    */
}

void Game::Logic::drawPlayerHeads(QImage* image)
{
    QPainter painter(image);
    painter.setRenderHint(QPainter::Antialiasing);
    for (const Player& player : players_)
        player.drawHead(&painter, currentIndex_, gameSettings_->sizePlayer);
}