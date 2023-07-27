#include "host.h"
#include "mainwindow.h"
/*
Host::Host(MainWindow *parent, Server* _server, QLabel* _statusLabel, QAction* _actionConnect) : 
    QWidget(parent), ui(new Ui::Host), server(_server), statusLabel(_statusLabel), actionConnect(_actionConnect)
{
    ui->setupUi(this);

    setWindowFlag(Qt::Window);
    //Wymusza wpisanie poprawnego adresu IPv4 (dlatego pytałem na zajęciach, czy potrzeba IPv6)
    QString ipNumber = "(([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))\\.";
    ui->addressEdit->setValidator( new QRegExpValidator( QRegExp('^' + ipNumber + ipNumber + ipNumber + "(([0-1][0-9][0-9]$)|(2[0-4][0-9]$)|(25[0-5]$))", Qt::CaseInsensitive), this));

    //Przycisk host wprowadza server w stan nasłuchiwania
    connect(ui->hostButton, &QPushButton::released, this, [this]
    {
        QString address = ui->addressEdit->text();
        if (!address.size())
            return;

        quint16 port = static_cast<quint16>(ui->portSpinBox->text().toInt());
        QHostAddress addr = QHostAddress(address);
        if (port < 1 || port > 65535)
        {
            QMessageBox::critical(this, tr("Error"),
                                  tr("Port number must be in range 1-65535"));
            return;
        }

        if (server->listen(QHostAddress(address), port))
        {
            actionConnect->setDisabled(true);
            statusLabel->setText("Waiting for players");

            static_cast<MainWindow *>(this->parent())->createMocks(addr, port, 3);
            close();
        }

        updateStatusLabel();
    });
}

void Host::updateStatusLabel()
{
    if (server->server.isListening())
    {
        if (ui->statusLabel)
            ui->statusLabel->setText("Already hosted on " + server->server.serverAddress().toString() + ":" + std::to_string(server->server.serverPort()).c_str());
        ui->hostButton->setDisabled(true);

    }
    else
    {
        if (ui->statusLabel)
            ui->statusLabel->setText("");
        ui->hostButton->setDisabled(false);
    }
}

Host::~Host()
{
    //QObject::~QObject();
}
*/