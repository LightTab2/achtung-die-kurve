#include "player.h"

#include <QPainter>
#include <QRandomGenerator>
#include <QtMath>
#include <gsl/gsl>

Player::Player(const QString& name__, bool bLocal__) 
    : name{ name__ },
      bLocal{ bLocal__ }
{
}
    
bool Player::operator==(const QString& playerName) const
{
    return this->name == playerName;
}

bool Player::operator!=(const QString& playerName) const
{
    return !(*this == playerName);
}

Player::operator bool() const
{
    return isValid();
}

QColor Player::getColor() const
{
    Ensures(colorIndex_ >= 0 && colorIndex_ < 8);
    if (bDead_)
        return Player::playerDeadColors[colorIndex_];
    return Player::playerColors[colorIndex_];
}

void Player::setColorIndex(int colorIndex)
{
    Expects(colorIndex >= 0 && colorIndex < 8);
    colorIndex_ = colorIndex;
}

void Player::startRotating(Direction direction)
{ 
    input_ |= (static_cast<bool>(direction) ? static_cast<quint8>(Input::right) : static_cast<quint8>(Input::left));
}

void Player::stopRotating(Direction direction)
{
    input_ &= ~(static_cast<bool>(direction) ? static_cast<quint8>(Input::right) : static_cast<quint8>(Input::left));
}

void Player::setNoDrawTicks(int noDrawTicks)
{
    noDrawTicks_ = noDrawTicks;
}

bool Player::isValid() const
{
    return (bActive && !bDead_);
}

void Player::reset(gsl::not_null<const Game::Settings*> settings) noexcept
{
    const int wallThickness = settings->wallThickness;
    const int sizePlayer    = settings->sizePlayer;
    const int timeoutTicks  = settings->getTimeoutTicks();
    const int sizeBoard     = settings->getBoardSize();

    Expects(wallThickness > 0);
    Expects(sizePlayer    > 0);
    Expects(timeoutTicks  > 1);
    Expects(sizeBoard     > ((wallThickness + sizePlayer) * 2 + 1));

    data_.resize(timeoutTicks);

    confirm_.clear();
    confirm_.resize(timeoutTicks);

    auto randomCoordinate = [&wallThickness, &sizeBoard, &sizePlayer]()
    {
        return QRandomGenerator::global()->bounded(wallThickness + sizePlayer, sizeBoard - wallThickness - sizePlayer);
    };

    DrawData& firstDrawData = data_[0].drawData;
    firstDrawData.lineEnd   = QPointF(randomCoordinate(), randomCoordinate());

    data_[0].angle = 2*M_PI * QRandomGenerator::global()->generateDouble();

    bDead_  = false;
    bActive = true;
}

void Player::drawHead(QPainter* painter, const WrappingData<int>& drawIndex, int playerSize) const
{
    if (!bActive)
        return;

    painter->setPen(getColor());
    painter->setBrush(getColor());

    // small optimization because division is not cheap
    static int lastPlayerSize         = playerSize;
    static float cachedHalfPlayerSize = playerSize/2.f;
    if (playerSize != lastPlayerSize)
    {
        lastPlayerSize       = playerSize;
        cachedHalfPlayerSize = playerSize/2.f;
    }
    // Draws Player's head
    const QPointF center = bDead_ ? deadPos_ : data_[drawIndex].drawData.lineEnd;
    painter->drawEllipse(center, cachedHalfPlayerSize, cachedHalfPlayerSize);
    //Rysuje strzałkę, która wskazuje kierunek gracza
    /*if (elapsed < GameSettings::FREEZE_TIME)
    {
        QPointF directionPoint = scale *
            QPointF(player.pos_x + static_cast<qreal>((static_cast<unsigned long>(GameSettings::PLAYER_SIZE) << 1u)) * qCos(player.angle), player.pos_y + static_cast<qreal>((static_cast<unsigned long>(GameSettings::PLAYER_SIZE) << 1u) * qSin(player.angle)));
        painter.drawLine(scale * QPointF(player.pos_x, player.pos_y),
            directionPoint);
        painter.drawLine(directionPoint,
            scale * QPointF(player.pos_x + static_cast<qreal>(GameSettings::PLAYER_SIZE) * 1.5 * qCos(player.angle - (M_PI_4 / 2)), player.pos_y + static_cast<qreal>(GameSettings::PLAYER_SIZE) * 1.5 * qSin(player.angle - (M_PI_4 / 2))));
        painter.drawLine(directionPoint,
            scale * QPointF(player.pos_x + static_cast<qreal>(GameSettings::PLAYER_SIZE) * 1.5 * qCos(player.angle + (M_PI_4 / 2)), player.pos_y + static_cast<qreal>(GameSettings::PLAYER_SIZE) * 1.5 * qSin(player.angle + (M_PI_4 / 2))));

        QFont               font("Arial", static_cast<int>(GameSettings::NAME_FONT_SIZE));
        QFontMetrics        fm(font);
        int                 pixelsWide = fm.horizontalAdvance(player.name.c_str());
        int                 pixelsHigh = fm.height();
        painter.setFont(font);

        painter.drawText(scale * QPointF(player.pos_x - (pixelsWide >> 1), qRound(player.pos_y) + pixelsHigh),
            tr(player.name.c_str()));
    }*/
}

void Player::drawLine(QPainter* painter, const WrappingData<int>& drawIndex, const QColor& color, int lineSize, Qt::PenCapStyle capStyle) const
{
    if (!isValid() || data_[drawIndex].drawData.bNoDraw)
        return;

    painter->setPen(QPen(color, lineSize, Qt::SolidLine, capStyle, Qt::MiterJoin));

    const QPointF& lineBeg = data_[drawIndex - 1].drawData.lineEnd;
    const QPointF& lineEnd = data_[drawIndex].drawData.lineEnd;
    // Draws Player's head
    painter->drawLine(lineBeg, lineEnd);
}

void Player::calculateNextMove(const WrappingData<int>& index, float speed, float rotationSpeed)
{
    if (!isValid())
        return;

    MovementData& prevData = data_[index - 1];
    MovementData& nextData = data_[index];

    DrawData& prevDrawData = prevData.drawData;
    DrawData& nextDrawData = nextData.drawData;

    nextDrawData.bNoDraw = noDrawTicks_ > 0;

    if (noDrawTicks_ > 0)
        --noDrawTicks_;

    nextData.angle = prevData.angle;
    if (input_ == static_cast<quint8>(Player::Input::left))
        nextData.angle -= rotationSpeed;
    else if (input_ == static_cast<quint8>(Player::Input::right))
        nextData.angle += rotationSpeed;

    nextDrawData.lineEnd = prevDrawData.lineEnd;
    nextDrawData.lineEnd += QPointF(speed * qCos(prevData.angle), speed * qSin(prevData.angle));
}

void Player::checkWallCollision(const WrappingData<int>& index, int boardSize, int wallThickness)
{
    if (!isValid())
        return;

    const qreal x = data_[index].drawData.lineEnd.x();
    const qreal y = data_[index].drawData.lineEnd.y();

    if (x <= wallThickness || y <= wallThickness || x >= boardSize - wallThickness || y >= boardSize - wallThickness)
        die(index);
}

void Player::die(gsl::index index)
{
    if (bDead_)
        return;

    bDead_   = data_[index].bDemise = true;
    deadPos_ = data_[index].drawData.lineEnd;
}


