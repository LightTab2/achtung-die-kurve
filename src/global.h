﻿#pragma once
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QElapsedTimer>
#include <QPixmap>
#include <QPaintEvent>
#include <QBuffer>
#include <QPainter>
#include <QTcpServer>
#include <QRandomGenerator>
#include <QColor>
#include <QPainter>
#include <QtMath>
#include <QTcpSocket>
#include <QUdpSocket>
#include <fstream>
#include <QAction>
#include <QNetworkDatagram>
#include <QDataStream>
//Paddings
//#pragma warning( disable : 4820 )
//connect
//#pragma warning( disable : 26444 )
//Spectre
//#pragma warning( disable : 5045 )
/*
class MainWindow;
class GameLogic;
class Connect;
class ManagedConnection;
class Host;
class Client;
class Server;

struct MessageType
{
    static const unsigned char ping         = 0x01;
    static const unsigned char name         = 0x02;
    static const unsigned char gameStart    = 0x03;
    static const unsigned char tickSync     = 0x04;     //synchronizuje start gry, tak by serwer i klient grali w tym samym czasie
    static const unsigned char playerSync   = 0x05;     //synchronizuje graczy
    static const unsigned char reconnect    = 0x06;
    static const unsigned char gameEnd      = 0x07;
    static const unsigned char sendPos      = 0x80;
    static const unsigned char resendPos    = 0x81;
    static const unsigned char sendInput    = 0x82;
};

//Stanowi prosty licznik, który powraca do 0, gdy przekroczy swój zakres
//Zlicza ile razy przekroczył swój zakres, co może być użyte do porównywania dwóch obiektów tej klasy
//Bardziej optymalna struktura, ale podatna na błędy (potrzeba lepszego testowania)
struct BoundUnsigned
{
    BoundUnsigned() {}
    BoundUnsigned(unsigned _boundary) : boundary(_boundary)
    {

    }

    BoundUnsigned& operator=(const unsigned rhs)
    {
        data = rhs;

        return *this;
    }

    BoundUnsigned& operator=(const BoundUnsigned rhs)
    {
        data    = rhs.data;
        compare = rhs.compare;
        boundary = rhs.boundary;
        return *this;
    }

    BoundUnsigned& operator++()
    {
        data += 1u;

        if (data == boundary)
        {
            data = 0;
            ++compare;
        }

        return *this;
    }

    BoundUnsigned& operator--()
    {
        if (data == 0u)
        {
            data = boundary - 1u;
            --compare;
        }
        else
            data -= 1u;
        return *this;
    }

    BoundUnsigned operator+(unsigned rhs)
    {
        BoundUnsigned newOne = *this;
        newOne.data += rhs;
        if (newOne.data >= boundary)
        {
            newOne.data -= boundary;
            ++newOne.compare;
        }
        return newOne;
    }

    BoundUnsigned operator-(unsigned rhs)
    {
        BoundUnsigned newOne = *this;
        if (rhs > newOne.data)
        {
            newOne.data = boundary - rhs + newOne.data;
            --newOne.compare;
        }
        else
            newOne.data -= rhs;
        return newOne;
    }

    friend QDataStream& operator<<(QDataStream& stream, const BoundUnsigned& variable)
    {
        stream  << variable.data << variable.compare;

        return stream;
    }

    friend QDataStream& operator>>(QDataStream& stream, BoundUnsigned& variable)
    {
        stream  >> variable.data >> variable.compare;

        return stream;
    }

    friend bool operator<(const BoundUnsigned& l, const BoundUnsigned& r)
    {
        if (l.compare == r.compare)
            return l.data < r.data;
        return l.compare < r.compare;
    }

    friend bool operator>(const BoundUnsigned& l, const BoundUnsigned& r)
    {
        if (l.compare == r.compare)
            return l.data > r.data;
        return l.compare > r.compare;
    }

    friend bool operator<=(const BoundUnsigned& l, const BoundUnsigned& r)
    {
        if (l.compare == r.compare)
            return l.data <= r.data;
        return l.compare <= r.compare;
    }

    friend bool operator>=(const BoundUnsigned& l, const BoundUnsigned& r)
    {
        if (l.compare == r.compare)
            return l.data >= r.data;
        return l.compare >= r.compare;
    }

    explicit operator unsigned() const { return data; }

    unsigned    data        = 0u,
                boundary    = 0u;

    int         compare     = 0;
};

*/