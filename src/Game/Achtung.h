#pragma once
#include <QObject>

#include <gsl/gsl>

#include "Game/Logic/GameLogic.h"
#include "Game/Data/LeaderboardModel.h"
#include "GameCanvas.h"

/// Joins \ref Game modules and manages their communication
///
class Achtung final : public QObject
{
    Q_OBJECT
public:
    // Constructor
    Achtung(QObject* parent = nullptr);
    // Disallow copy
    Achtung(const Achtung& obj)            = delete;
    Achtung& operator=(const Achtung& rhs) = delete;
    // Allow move
    Achtung(Achtung&& obj)        noexcept = default;
    Achtung& operator=(Achtung&&) noexcept = default;
    // Virtual destructor
    virtual ~Achtung() = default;

    ///
    /// \param gameCanvas Takes GameCanvas pointer to connect it with the \ref Logic::boardLocal_ for drawing
    void setDrawingTarget(gsl::not_null<GameCanvas*> gameCanvas);

    void start();
    void stop();

    Game::Settings settings;

signals:
    void requestAnimation();
    void applySettings(int boardSize);
    void keyPressed(Qt::Key key);
    void keyReleased(Qt::Key key);

private:
    Game::Logic logic_{&settings};
};