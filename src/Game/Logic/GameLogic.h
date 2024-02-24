#pragma once

#include <QElapsedTimer>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QThread>
#include <gsl/gsl>

#include "Game/Data/Settings/GameSettings.h"
#include "Game/Data/Player/player.h"

namespace Game
{
    /// All the Game logic and drawing - both server-side and client-side - is contained within this class.
    ///
    class Logic final : public QObject
    {
        Q_OBJECT
    public:
        // Constructor
        Logic(gsl::not_null<Settings*> gameSettings);
        // Disallow copy
        Logic(const Logic& obj) = delete;
        Logic& operator=(const Logic&) = delete;
        // Allow move
        Logic(Logic&& obj)        noexcept = default;
        Logic& operator=(Logic&&) noexcept = default;
        // Destructor
        ~Logic() = default;

        bool startGame();
        //Powoduje wyłonienie zwyciężcy i odliczanie czasu do końca gry
        void endGame();
        //Wyłącza grę (nie aplikację)
        void stopGame();

        const QImage* getBoardImage() const;

    public slots:
        void drawTickTransition();
        void localPlayerKeyPressed(Qt::Key key);
        void localPlayerKeyReleased(Qt::Key key);
        void drawPlayerHeads(QImage* image);

    signals:
        void applySettings(int boardSize);

    private:
        void tick();
        void checkWin();

        //Losowanie, czy dany gracz przez określony czas nie będzie rysowany i powstanie dziura w jego linii
        void rollNoDraw(Player& player);
        void checkPlayerCollision(const WrappingData<int>& index);

        //Sieć
        void readDatagramsClient();
        void readDatagramsServer();
        void sendPosition();
        void resendPosRequest();
        void resendPos();

        //QByteArray abuildRepositionMessage  (std::vector<ManagedConnection::ResendData>& resend);
        //void resendPosition();

        //Istnieje konwersja z const char* na QByteArray, więc można wygodnie szukać gracza po nazwie
        Player* findPlayer(const QString& playerName) const;

        //Grafika
        void updateMap();

        bool bWon_ = false;        //Czy znamy zwyciężcę i czekamy na nową rundę
        bool bRunning_ = false;        //Mówi, czy gra trwa i czy jest sens np. rysować okienko
        //bool bServer_  = false;

        QImage board_;             //Obraz, na którym klient przewiduje następny stan gry
        QImage boardSafety_;
        QImage boardCollision_;

        QString status_;
        QColor  statusColor_;

        QString winner_;

        WrappingData<int> currentIndex_;
        WrappingData<int> confirmIndex_;

        std::vector<Player> players_;

        QElapsedTimer gameStartETimer_;


        gsl::not_null<Settings*> gameSettings_;

        std::unique_ptr<QThread> tickThread_;
        int tick_ = 0;
        double tickInterval_ = 0.0;
        double nextTick_ = 0.0;
    };
}