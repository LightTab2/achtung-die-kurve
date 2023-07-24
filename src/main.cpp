#include "mainwindow.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine; 
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/achtung_die_kurve/mainWindow.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
