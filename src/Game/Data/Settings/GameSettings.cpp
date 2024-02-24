#pragma once
#include "GameSettings.h"

namespace Game
{
    Settings::Settings(int tickRate__, int sizeBoard__, int sizeLine__, int sizePlayer__, int wallThickness__, double speed_percents__, double speedRotation_percents__, double timeout_seconds__, double grace_seconds__, double freeze_seconds__, double winWait_seconds__, double gapChance_percents__, double gapMin_seconds__, double gapMax_seconds__)
        : TickSubSettings(tickRate__, speed_percents__, speedRotation_percents__, timeout_seconds__, grace_seconds__, freeze_seconds__, winWait_seconds__, gapChance_percents__, gapMin_seconds__, gapMax_seconds__),
          sizeBoard_{ sizeBoard__ },
          sizeLine{ sizeLine__ },
          sizePlayer{ sizePlayer__ },
          wallThickness{ wallThickness__ }
    {
        Expects(tickRate_     > 0 && tickRate_ <= 1000);
        Expects(sizeBoard_    > ((wallThickness + sizePlayer) * 2 + 1));
        Expects(sizeLine      > 0);
        Expects(sizePlayer    > 0);
        Expects(wallThickness > 0);

        setBoardSize(sizeBoard_);
        setTickRate(tickRate_);
    }

    int Settings::getBoardSize() const
    {
        return sizeBoard_;
    }

    void Settings::setBoardSize(int size)
    {
        Expects(size > ((wallThickness + sizePlayer) * 2 + 1));

        sizeBoard_ = size;
        fontSize_  = qMin(1, qRound(sizeBoard_/8.f));

        Ensures(fontSize_ > 0);
    }

    int Settings::getFontSize() const
    {
        return fontSize_;
    }

    QDataStream& operator<<(QDataStream& stream, const Settings& data)
    {
        const TickSubSettings& subSettings = data;
        stream << subSettings
               
               << data.sizeBoard_;
  
        return stream;
    }

    QDataStream& operator>>(QDataStream& stream, Settings& data)
    {
        TickSubSettings& subSettings = data;

        stream >> subSettings

               >> data.sizeBoard_;
  
        data.setBoardSize(data.sizeBoard_);
        data.setTickRate(data.tickRate_);

        return stream;   
    }
}