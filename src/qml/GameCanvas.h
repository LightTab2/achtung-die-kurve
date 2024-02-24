#pragma once
#include <QQuickPaintedItem>

#include <gsl/gsl>

#include <Game/Data/Settings/GameSettings.h>

class GameCanvas : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

public:
    Q_PROPERTY(int boardSize MEMBER boardSize_ NOTIFY boardSizeChanged)

    // Constructor
    GameCanvas(QQuickItem* parent = nullptr) noexcept;
    // Disallow copy
    GameCanvas(const GameCanvas&)            = delete;
    GameCanvas& operator=(const GameCanvas&) = delete;
    // Allow move
    GameCanvas(GameCanvas&&)            noexcept = default;
    GameCanvas& operator=(GameCanvas&&) noexcept = default;
    // Virtual destructor
    virtual ~GameCanvas() = default;

    void setRenderImage(gsl::not_null<const QImage*> boardImage);

    void paint(QPainter* painter) override;

public slots:
    void updateBoardSize(int boardSize);

signals: 
    void boardSizeChanged();
    void drawHeads(QImage* image);

protected:
    void geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry) override;

private:
    /// Image that will be painted on the canvas. 
    ///
    /// It should contain all the game drawn elements: board, player's trails, powerups and status messages
    const QImage* boardImage_ = nullptr;

    QRectF boardRect_{};

    int boardSize_ = 512;
};