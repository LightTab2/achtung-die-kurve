#include "MainWindow.h"

MainWindow::MainWindow(QWindow *parent) : QQuickWindow(parent)/*, ui(new Ui::MainWindow),
        host((ui->setupUi(this), this), &server, ui->statusLabel, ui->actionConnect),
        con(this,       &client, ui->statusLabel, ui->actionHost),
        server(this,    &host, &logic, &data, ui->statusLabel, ui->actionConnect),
        client(this,    &con, &logic, &data, ui->statusLabel, ui->actionHost),
        logic(this,     &server, &client, &data, ui->statusLabel),
        data(this)*/
{
    /*this->installEventFilter(this);
    ui->gameWindow->setGame(&logic, &data);
    connect(ui->actionHost,         &QAction::triggered, this, [this] { con.close(); host.show(); });
    connect(ui->actionDisconnect,   &QAction::triggered, this, [this] { server.close(); client.close(); host.updateStatusLabel(); });
    connect(ui->actionConnect,      &QAction::triggered, this, [this] { host.close(); con.show(); });
    con.close();
    host.close();*/
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    //this->ui->gameWindow->resize(this->size() - QSize(20, 60));
    //this->ui->statusLabel->resize(this->size().width() - 20, 30);
    QWindow::resizeEvent(event);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        //client.keyPressed(static_cast<QKeyEvent*>(event)->key());
        return true;
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        //client.keyReleased(static_cast<QKeyEvent*>(event)->key());
        return true;
    }
    return false;
}

MainWindow::~MainWindow()
{
    //QMainWindow::~QMainWindow();
}

//Pomocniczna funkcja tworząca więcej okienek, przy pomocy których można testować program
//Jest automatyczne łączenie (po to są parametry address i port)
void MainWindow::createMocks(QHostAddress address, quint16 port, unsigned number)
{
    if (this->parent())
        return;
    /*for (unsigned i = 0; i != number; ++i)
    {
        mockClients.emplace_back(new MainWindow(this));
        mockClients.back()->setAttribute(Qt::WA_DeleteOnClose);
        mockClients.back()->show();
        mockClients.back()->client.connect(std::to_string(i), address, port);
    }*/
}
