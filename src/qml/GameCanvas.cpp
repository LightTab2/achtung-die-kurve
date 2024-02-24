#include "qml/GameCanvas.h"

#include <QPainter>

GameCanvas::GameCanvas(QQuickItem* parent) noexcept
    : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
}

void GameCanvas::setRenderImage(gsl::not_null<const QImage*> boardImage)
{
    boardImage_ = boardImage;
}

void GameCanvas::updateBoardSize(int boardSize)
{
    boardSize_ = boardSize;
    emit boardSizeChanged();
}

#include <QElapsedTimer>
#include <iostream>
void GameCanvas::paint(QPainter* painter)
{
    { //debug
        static QElapsedTimer elapsed;
        static int fps = 0;
        ++fps;
        if (!elapsed.isValid())
            elapsed.start();
        if (elapsed.elapsed() > 1000)
        {
            elapsed.start();
            std::cout << "fps: " << fps << std::endl;
            fps = 0;
        }
    }

    if (boardImage_)
    {
        // Copy the Image
        static QImage boardCopyImage;
        boardCopyImage = boardImage_->copy();
        // And draw heads on top of it
        emit drawHeads(&boardCopyImage);
        // Draw image drawn by Game's logic
        if (boardImage_)
            painter->drawImage(boardRect_, boardCopyImage);
    }
}

void GameCanvas::geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    int size = qMin(qRound(newGeometry.width()), boardSize_);
    size = qMin(qRound(newGeometry.height()), size);
    boardRect_.setWidth(size);
    boardRect_.setHeight(size);

    QQuickPaintedItem::geometryChange(newGeometry, oldGeometry);
}