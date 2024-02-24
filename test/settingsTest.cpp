#include <QTest>
#include "GameSettings.h"

class GameSettingsTest : public QObject
{
    Q_OBJECT
private slots:
    void checkConstructor();
    void checkExceptions();
private:
    Game::Settings settings;
};

void GameSettingsTest::checkConstructor()
{
    settings = Game::Settings(,);
}

QTEST_MAIN(GameSettingsTest)
#include "settingsTest.moc"