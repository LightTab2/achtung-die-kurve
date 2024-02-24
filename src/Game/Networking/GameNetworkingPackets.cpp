#include "Game/Data/Player/Player.h"

Player::DrawData::DrawData(QPointF lineEnd__, bool bNoDraw__)
    : lineEnd(lineEnd__),
      bNoDraw(bNoDraw__)
{
}

Player::MovementData::MovementData(DrawData drawData__, float angle__, bool bDemise__, int noDrawTicks) 
    : drawData(drawData__),
      angle(angle__),
      bDemise(bDemise__)
{
    if (noDrawTicks != -1)
        this->noDrawTicks = noDrawTicks;
}


QDataStream& operator<<(QDataStream& stream, const Player::DrawData& data)
{
    stream << data.lineEnd
           << data.bNoDraw;

    return stream;
}

QDataStream& operator>>(QDataStream& stream, Player::DrawData& data)
{
    stream >> data.lineEnd
           >> data.bNoDraw;

    return stream;
}

QDataStream& operator<<(QDataStream& stream, const Player::MovementData& data)
{
    stream << data.drawData
           << data.angle
           << data.bDemise
           << data.noDrawTicks.value_or(-1);

    return stream;
}

QDataStream& operator>>(QDataStream& stream, Player::MovementData& data)
{
    int noDrawTicks = -1;

    stream >> data.drawData
           >> data.angle
           >> data.bDemise
           >> noDrawTicks;

    if (noDrawTicks != -1)
        data.noDrawTicks = noDrawTicks;

    return stream;
}