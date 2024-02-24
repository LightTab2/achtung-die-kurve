
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick>
#include <iostream>

#include "MainWindow.h"

#include "Exceptions.h"

int main(int argc, char *argv[])
{
    int returnCode = -1;
    try
    {
        qInstallMessageHandler(errorMessageHandler);
        QGuiApplication app(argc, argv);
        QQmlApplicationEngine engine; 
        LeaderboardModel leaderboardModel;
        engine.load(QUrl(QStringLiteral("qrc:/qt/qml/achtung_die_kurve/Main.qml")));
        if (engine.rootObjects().isEmpty())
            qCritical() << ErrorType::General << "Couldn't load \"Main.qml\"";
        
        QObject* rootObject = engine.rootObjects()[0];
        MainWindow* mainWindow = qobject_cast<MainWindow*>(rootObject);
        if (!mainWindow)
            qCritical() << ErrorType::General << "Problem loading the QML file: Root object is not a MainWindow. Possibly corrupt \"Main.qml\" file";
        
        mainWindow->setupExceptionDialog();
        mainWindow->setupDrawingCanvas();

        QObject* leaderboard= mainWindow->findChild<QObject*>(QStringLiteral("leaderboard"));
        if (!leaderboard)
            throw AppException("Problem loading QML file: Couldn't find the ListView responsible for drawing the leaderboards. Possibly corrupt \"Main.qml\" file");

        leaderboard->setProperty("model", QVariant::fromValue(&leaderboardModel));

        app.setWindowIcon(QIcon(QStringLiteral(":/qt/qml/achtung_die_kurve/icon/icon_256x256.png")));

        mainWindow->debugStart();

        returnCode = app.exec();
    }
    catch (AppException& ex)
    {
        std::cerr << ex.what();
        ERROR_MESSAGE(ex.what());
        throw;
    }
    catch (QException& ex)
    {
        std::cerr << "[Critical] Unidentified \"QException\" has occurred: " << ex.what();
        std::string msg = "Unidentified \"QException\" has occurred:\n";
        msg += ex.what();
        ERROR_MESSAGE(msg.c_str());
        throw;
    }
    catch (std::exception& ex)
    {
        std::cerr << "[Critical] Unidentified \"std::exception\" has occurred: " << ex.what();
        std::string msg = "Unidentified \"std::exception\" has occurred:\n";
        msg += ex.what();
        ERROR_MESSAGE(msg.c_str());
        throw;
    }
    catch (...)
    {
        std::cerr << "[Critical] Unidentified exception has occurred\n";
        ERROR_MESSAGE("Unidentified exception has occurred");
        throw;
    }
    return returnCode;
}
