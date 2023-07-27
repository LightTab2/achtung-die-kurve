#pragma once
#include <QtQuick>

class MainWindow : public QQuickWindow
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
    //std::unique_ptr<Ui::MainWindow>             ui;
    //std::vector<std::unique_ptr<MainWindow> >   mockClients;        //Debug

    /*Host        host;
    Connect     con;

    Server      server;
    Client      client;

public:
    GameLogic   logic;
    GameData    data;*/
};
