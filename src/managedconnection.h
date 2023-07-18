#pragma once
#include "global.h"
#include "player.h"

/*
class Player;
class ManagedConnection
{
public:
    //Wskazuje na dane, które serwer musi wysłać ponownie w wyniku niedotarcia pakietku UDP
    struct ResendData
    {
        ResendData() {}
        ResendData(Player* player) : player(player)     {}

        ResendData(const ResendData &resend)            { *this = resend; }

        ResendData& operator=(const ResendData &rhs)
        {
            this->player                = rhs.player;
            this->confirm               = rhs.confirm;
            return *this;
        }

        Player                  *player = nullptr;
        std::vector<unsigned>   confirm;
    };

    ManagedConnection(QTcpSocket *socket) : socket(socket)
    {
        lastReceived.start();
        address = socket->localAddress().toIPv4Address();
        port    = socket->localPort();
    }

    void    connect         (QHostAddress address, unsigned short port);
    bool    bindUdpSocket   (unsigned short port);
    void    sendData        (const unsigned char msgType, QByteArray msg = "");
    void    sendDatagram    (const unsigned char msgType, QByteArray msg = "", unsigned short destPort = 0);
    void    sendDatagram    (const unsigned char msgType, QByteArray msg, quint32 destAddr, unsigned short destPort);
    bool    readData        (unsigned char &msgType, QByteArray &msg);
    bool    readDatagram    (unsigned char &msgType, QByteArray &msg, quint32 *sender = nullptr, unsigned short *senderPort = nullptr);
    void    reset           ();
    void    close           ();
    Player* getPlayer       (std::vector<Player> &players);

    QTcpSocket                  *socket         = nullptr;
    std::shared_ptr<QUdpSocket> udpSocket       = std::shared_ptr<QUdpSocket>(new QUdpSocket());
    quint32                     address         = 0u,
                                token           = 0u;
    unsigned short              port            = 0u,
                                udpPort         = 0u;
    bool                        bConnected      = false;

    QByteArray                  data;
    QElapsedTimer               lastReceived;
    std::string                 playerName      = "";
};
*/