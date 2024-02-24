#include "MainWindow.h"

#include "Exceptions.h"

MainWindow::MainWindow(QWindow* parent)
    : QQuickWindow(parent)/*, ui(new Ui::MainWindow),
        host((ui->setupUi(this), this), &server, ui->statusLabel, ui->actionConnect),
        con(this,       &client, ui->statusLabel, ui->actionHost),
        server(this,    &host, &logic, &data, ui->statusLabel, ui->actionConnect),
        client(this,    &con, &logic, &data, ui->statusLabel, ui->actionHost),
        logic(this,     &server, &client, &data, ui->statusLabel),
        data(this)*/
{
    this->installEventFilter(this);
    this->setIcon(QIcon(QStringLiteral(":/qt/qml/achtung_die_kurve/icon/icon_256x256.png")));
    this->setTitle(QStringLiteral("Achtung Die Kurve!"));
    connect(this, &MainWindow::requestAnimation, &achtung_, &Achtung::requestAnimation);
    connect(this, &MainWindow::keyPressed,       &achtung_, &Achtung::keyPressed);
    connect(this, &MainWindow::keyReleased,      &achtung_, &Achtung::keyReleased);
    /*ui->gameWindow->setGame(&logic, &data);
    connect(ui->actionHost,         &QAction::triggered, this, [this] { con.close(); host.show(); });
    connect(ui->actionDisconnect,   &QAction::triggered, this, [this] { server.close(); client.close(); host.updateStatusLabel(); });
    connect(ui->actionConnect,      &QAction::triggered, this, [this] { host.close(); con.show(); });
    con.close();
    host.close();*/
}

void MainWindow::setupExceptionDialog()
{
    AppException::exceptionMessage = findChild<QObject*>(QStringLiteral("exceptionMessage"));
    if (!AppException::exceptionMessage)
        qCritical() << ErrorType::General << "Problem loading the QML file: Couldn't find the Dialog responsible for displaying exception messages. Possibly corrupt \"Main.qml\" file";
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type()) 
    {
        case QEvent::KeyPress:
            emit keyPressed(static_cast<Qt::Key>( (static_cast<QKeyEvent*>(event))->key() ));
            return true;
        break;

        case QEvent::KeyRelease:
            emit keyReleased(static_cast<Qt::Key>( (static_cast<QKeyEvent*>(event))->key() ));
            return true;
        break;

        case QEvent::UpdateRequest:
            if (isExposed())
            {
                emit requestAnimation();
                if (gameCanvas_)
                    gameCanvas_->update();
            }
            requestUpdate();
        break;

        default:
        break;
    }
    return false;
}

MainWindow::~MainWindow()
{
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

void MainWindow::setupDrawingCanvas()
{
    QObject* gameCanvasQObject = findChild<QObject*>(QStringLiteral("gameCanvas"));
    if (!gameCanvasQObject)
        throw AppException("Problem loading QML file: Couldn't find the GameCanvas responsible for drawing the game. Possibly corrupt \"Main.qml\" file");

    gameCanvas_ = qobject_cast<GameCanvas*>(gameCanvasQObject);
    if (!gameCanvas_)
        throw AppException("Problem loading QML file: Object with name \"gameCanvas\" was found, but is not of GameCanvas type. Possibly corrupt \"Main.qml\" file");

    achtung_.setDrawingTarget(gameCanvas_);
}

void MainWindow::debugStart()
{
    achtung_.start();
}
