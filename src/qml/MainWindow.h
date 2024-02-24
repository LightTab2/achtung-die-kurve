#pragma once

#include <QtQuick>

#include "Game/Achtung.h"
#include "GameCanvas.h"

/// Represents the window created on the application start
///
class MainWindow : public QQuickWindow
{
    Q_OBJECT
    QML_ELEMENT

public:
    // Constructor
    MainWindow(QWindow* parent = nullptr);
    // Disallow copy
    MainWindow(const MainWindow&)            = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    // Allow move
    MainWindow(MainWindow&&)            noexcept = default;
    MainWindow& operator=(MainWindow&&) noexcept = default;
    // Destructor
    virtual ~MainWindow() override;

    // Debug
    void createMocks(QHostAddress address, quint16 port, unsigned number);

    void setupDrawingCanvas();

    void setupExceptionDialog();

    void debugStart();

signals:
    void requestAnimation();
    void keyPressed(Qt::Key key);
    void keyReleased(Qt::Key key);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Achtung achtung_{this};
    GameCanvas* gameCanvas_ = nullptr;
};
