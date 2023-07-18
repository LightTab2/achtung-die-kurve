#include <QTest>
#include "mainwindow.h"

class MainWindowTest : public QObject
{
    Q_OBJECT
private slots:
    void toUpper();
private:
    MainWindow win { nullptr };
};

void MainWindowTest::toUpper()
{
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/achtung-die-kurve/mainWindow.qml")));
    QCOMPARE(engine.rootObjects().isEmpty(), false);
}


QTEST_MAIN(MainWindowTest)
#include "testTest.moc"