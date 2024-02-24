#include "GameSettings.h"

namespace Game
{
    TickSubSettings::TickSubSettings(int tickRate__, double speed_percents__, double speedRotation_percents__, double timeout_seconds__, double grace_seconds__, double freeze_seconds__, double winWait_seconds__, double gapChance_percents__, double gapMin_seconds__, double gapMax_seconds__, double speedPowerUp_seconds__, int speedPowerUpMaxLevel__, double speedPowerUpMultiplier__)
        : tickRate_{ tickRate__ },
          speed_percents_{ speed_percents__ },
          speedRotation_percents_{ speedRotation_percents__ },
          timeout_seconds_{ timeout_seconds__ },
          grace_seconds_{ grace_seconds__ },
          freeze_seconds_{ freeze_seconds__ },
          winWait_seconds_{ winWait_seconds__ },
          gapChance_percents_{ gapChance_percents__ },
          gapMin_seconds_{ gapMin_seconds__ },
          gapMax_seconds_{ gapMax_seconds__ },
          speedPowerUp_seconds_{ speedPowerUp_seconds__ },
          speedPowerUpMaxLevel_{ speedPowerUpMaxLevel__ },
          speedPowerUpMultiplier_{ speedPowerUpMultiplier__ }
    {
        setTickRate(tickRate_);
    }

    int TickSubSettings::getTickTiedVar(double timeVariable, int tickRate)
    {
        Expects(tickRate > 0 && tickRate <= 1000);
        Expects(timeVariable >= 0.0);

        int rescaledVariable = qMax(1, qRound(timeVariable * tickRate));

        return rescaledVariable;
    }

    void TickSubSettings::setTickRate(int tickRate)
    {
        Expects(tickRate > 0 && tickRate <= 1000);

        tickRate_ = tickRate;

        setSpeedPercents(speed_percents_);
        setRotationSpeedPercents(speedRotation_percents_);
        setTimeoutTime(timeout_seconds_);
        setGraceTime(grace_seconds_);
        setFreezeTime(freeze_seconds_);
        setWinWaitTime(winWait_seconds_);
        setGapChance(gapChance_percents_);
        setGapMinTime(gapMin_seconds_);
        setGapMaxTime(gapMax_seconds_);
        setSpeedPowerUpTime(speedPowerUp_seconds_);
        setSpeedPowerUpMaxLevel(speedPowerUpMaxLevel_);
        setSpeedPowerUpMultiplier(speedPowerUpMultiplier_);
    }
    int TickSubSettings::getTickRate() const
    {
        return tickRate_;
    }

    float TickSubSettings::getSpeedPerTick() const
    {
        return speed_;
    }
    double TickSubSettings::getSpeedPercents() const
    {
        return speed_percents_;
    }
    void TickSubSettings::setSpeedPercents(double speed_percents)
    {
        Expects(speed_percents > 0.0);

        speed_percents_ = speed_percents;
        speed_          = gsl::narrow_cast<float>(DEFAULT_SPEED_ * speed_percents_/100.0 / tickRate_);

        rescaleSafetyTicksPerSpeedPowerUp();

        Ensures(speed_ > 0.f);
    }

    float TickSubSettings::getRotationSpeedPerTick() const
    {
        return speedRotation_;
    }
    double TickSubSettings::getRotationSpeedPercents() const
    {
        return speedRotation_;
    }
    void TickSubSettings::setRotationSpeedPercents(double speedRotation_percents)
    {
        Expects(speedRotation_percents > 0.0);

        speedRotation_percents_ = speedRotation_percents;
        speedRotation_          = gsl::narrow_cast<float>(DEFAULT_ROTATION_SPEED_ * speedRotation_percents_/100.0 / tickRate_);

        Ensures(speed_ > 0.f);

    }

    int TickSubSettings::getTimeoutTicks() const
    {
        return ticksTimeout_;
    }
    double TickSubSettings::getTimeoutTime() const
    {
        return timeout_seconds_;
    }
    void TickSubSettings::setTimeoutTime(double timeout_seconds)
    {
        ticksTimeout_ = TickSubSettings::getTickTiedVar(timeout_seconds, tickRate_);
        timeout_seconds_ = timeout_seconds;
    }

    int TickSubSettings::getGraceTicks() const
    {
        return ticksGrace_;
    }
    double TickSubSettings::getGraceTime() const
    {
        return grace_seconds_;
    }
    void TickSubSettings::setGraceTime(double grace_seconds)
    {
        ticksGrace_ = TickSubSettings::getTickTiedVar(grace_seconds, tickRate_);
        grace_seconds_ = grace_seconds;
    }

    int TickSubSettings::getFreezeTicks() const
    {
        return ticksFreeze_;
    }
    double TickSubSettings::getFreezeTime() const
    {
        return freeze_seconds_;
    }
    void TickSubSettings::setFreezeTime(double freeze_seconds)
    {
        ticksFreeze_ = TickSubSettings::getTickTiedVar(freeze_seconds, tickRate_);
        freeze_seconds_ = freeze_seconds;
        ticksWinWait_ += ticksFreeze_;
    }

    int TickSubSettings::getWinWaitTicks() const
    {
        return ticksWinWait_;
    }
    double TickSubSettings::getWinWaitTime() const
    {
        return winWait_seconds_;
    }
    void TickSubSettings::setWinWaitTime(double winWait_seconds)
    {
        ticksWinWait_ = TickSubSettings::getTickTiedVar(winWait_seconds, tickRate_);
        winWait_seconds_ = winWait_seconds;
    }

    QDataStream& operator<<(QDataStream& stream, const TickSubSettings& data)
    {
        stream << data.tickRate_
               
               << data.speed_percents_
               << data.speedRotation_percents_

               << data.timeout_seconds_
               << data.grace_seconds_
               << data.freeze_seconds_
               << data.winWait_seconds_
               
               << data.gapChance_percents_
               << data.gapMin_seconds_
               << data.gapMax_seconds_;

        return stream;
    }

    QDataStream& operator>>(QDataStream& stream, TickSubSettings& data)
    {
        stream >> data.tickRate_
               
               >> data.speed_percents_
               >> data.speedRotation_percents_

               >> data.timeout_seconds_
               >> data.grace_seconds_
               >> data.freeze_seconds_
               >> data.winWait_seconds_ 

               >> data.gapChance_percents_
               >> data.gapMin_seconds_
               >> data.gapMax_seconds_;

        data.setTickRate(data.tickRate_);

        return stream;
    }

    double TickSubSettings::getGapChancePerTick() const
    {
        return tickGapChance_;
    }
    double TickSubSettings::getGapChancePercents() const
    {
        return gapChance_percents_;
    }
    void TickSubSettings::setGapChance(double chance_percents)
    {
        Expects(chance_percents >= 0.0 && chance_percents <= 100.0);

        gapChance_percents_ = chance_percents;
        tickGapChance_      = chance_percents / tickRate_ / 100.0;

        Ensures(tickGapChance_ >= 0.0);
    }

    int TickSubSettings::getGapMinTicks() const
    {
        return ticksGapMin_;
    }
    double TickSubSettings::getGapMinTime() const
    {
        return gapMin_seconds_;
    }
    void TickSubSettings::setGapMinTime(double min_seconds)
    {
        ticksGapMin_    = TickSubSettings::getTickTiedVar(min_seconds, tickRate_);
        gapMin_seconds_ = min_seconds;

        if (gapMin_seconds_ > gapMax_seconds_)
        {
            gapMax_seconds_ = gapMin_seconds_;
            ticksGapMax_    = ticksGapMin_;
        }
    }

    int TickSubSettings::getGapMaxTicks() const
    {
        return ticksGapMax_;
    }
    double TickSubSettings::getGapMaxTime() const
    {
        return gapMax_seconds_;
    }
    void TickSubSettings::setGapMaxTime(double max_seconds)
    {
        ticksGapMax_    = TickSubSettings::getTickTiedVar(max_seconds, tickRate_);
        gapMax_seconds_ = max_seconds;

        if (gapMax_seconds_ < gapMin_seconds_)
        {
            gapMin_seconds_ = gapMax_seconds_;
            ticksGapMin_    = ticksGapMax_;
        }
    }

    double TickSubSettings::getSpeedPowerUpTime() const
    {
        return speedPowerUp_seconds_;
    }
    void TickSubSettings::setSpeedPowerUpTime(double speedPowerUp_seconds)
    {
        ticksSpeedPowerUp_ = TickSubSettings::getTickTiedVar(speedPowerUp_seconds, tickRate_);
        speedPowerUp_seconds_ = speedPowerUp_seconds;

        rescaleSafetyTicksPerSpeedPowerUp();
    }

    int TickSubSettings::getSpeedPowerUpMaxLevel() const
    {
        return speedPowerUpMaxLevel_;
    }
    void TickSubSettings::setSpeedPowerUpMaxLevel(int maxSpeedPowerUps)
    {
        Expects(maxSpeedPowerUps > 0);
        speedPowerUpMaxLevel_ = maxSpeedPowerUps;

        rescaleSafetyTicksPerSpeedPowerUp();
    }

    double TickSubSettings::getSpeedPowerUpMultiplier() const
    {
        return speedPowerUpMultiplier_;
    }
    void TickSubSettings::setSpeedPowerUpMultiplier(double speedPowerUpMultiplier)
    {
        Expects(speedPowerUpMultiplier >= 0.0);
        speedPowerUpMultiplier_ = speedPowerUpMultiplier_;

        rescaleSafetyTicksPerSpeedPowerUp();
    }

    int TickSubSettings::getSafetyTicks(gsl::index speedPowerup)
    {
        return safetyTicksPerSpeedPowerUp_[speedPowerup];
    }

    void TickSubSettings::rescaleSafetyTicksPerSpeedPowerUp()
    {
        safetyTicksPerSpeedPowerUp_.resize(speedPowerUpMaxLevel_);
        double speed = speed_percents_/100.0;
        for (gsl::index i = 0; i != speedPowerUpMaxLevel_; ++i)
        {
            safetyTicksPerSpeedPowerUp_[i] = qMin(getTimeoutTicks(), qMax(1, qRound(tickRate_ * DEFAULT_SAFETY_TICKS_RATIO_ / speed)));
            speed += speedPowerUpMultiplier_/100.0;
        }
    }
}