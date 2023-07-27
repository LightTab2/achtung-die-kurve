#pragma once
#include <QtQuick>

class MainWindow final : public QQuickWindow
{
public:
                MainWindow  (QWindow *parent = nullptr);
                ~MainWindow ()                                      override;

    //Debug
    void        createMocks (QHostAddress address, quint16 port, unsigned number);
    
    //Deleted
    MainWindow(MainWindow&) = delete;
    MainWindow& operator=   (const MainWindow) = delete;
protected:
    bool        eventFilter (QObject *watched, QEvent *event)       override;
    void        resizeEvent (QResizeEvent *event)                   override;

private:
    void        gameTick    ();

    /*Host        host;
    Connect     con;

    Server      server;
    Client      client;

public:
    GameLogic   logic;
    GameData    data;*/
};
