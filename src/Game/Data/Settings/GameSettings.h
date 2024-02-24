#pragma once

#include <QDataStream>
#include <QtMath>

#include <gsl/gsl>
#include <vector>

namespace Game
{
    class TickSubSettings
    {
        friend QDataStream& operator<<(QDataStream& stream, const TickSubSettings& data);
        friend QDataStream& operator>>(QDataStream& stream, TickSubSettings& data);
    public:
        // Constructor
        /// 
        /// \param tickRate \copybrief tickRate_
        /// \param speed_percents \copybrief speed_percents_
        /// \param speedRotation_percents \copybrief speedRotation_percents_
        /// \param timeout_seconds \copybrief timeout_seconds_
        /// \param grace_seconds \copybrief grace_seconds_
        /// \param freeze_seconds \copybrief freeze_seconds_
        /// \param winWait_seconds \copybrief winWait_seconds_
        /// \param gapChance_percents \copybrief gapChance_percents_
        /// \param gapMin_seconds \copybrief gapMin_seconds_
        /// \param gapMax_seconds \copybrief gapMax_seconds_
        /// \param speedPowerUp_seconds \copybrief speedPowerUp_seconds_
        /// \param maxSpeedPowerUps \copybrief speedPowerUpMaxLevel_
        /// \param speedPowerUpMultiplier \copybrief speedPowerUpMultiplier_
        /// \pre Expects(tickRate > 0 && tickRate <= 1000);
        /// \pre Expects(speed_percents >= 0.0);
        /// \pre Expects(speedRotation_percents >= 0.0);
        /// \pre Expects(timeout_seconds >= 0.0);
        /// \pre Expects(grace_seconds >= 0.0);
        /// \pre Expects(freeze_seconds >= 0.0);
        /// \pre Expects(winWait_seconds >= 0.0);
        /// \pre Expects(gapChance_percents >= 0.0 && gapChance_percents <= 100.0);
        /// \pre Expects(gapMin_seconds >= 0.0);
        /// \pre Expects(gapMax_seconds >= 0.0);
        /// \pre Expects(speedPowerUp_seconds >= 0.0);
        /// \pre Expects(maxSpeedPowerUps > 0);
        /// \pre Expects(speedPowerUpMultiplier >= 0.0);
        TickSubSettings(int tickRate = 30, double speed_percents = 100.0, double speedRotation_percents = 100.0, double timeout_seconds = 10.0, double grace_seconds = 4.0, double freeze_seconds = 3.0, double winWait_seconds = 3.0, double gapChance_percents = 39.3, double gapMin_seconds = 0.33, double gapMax_seconds = 0.66, double speedPowerUp_seconds = 3.0, int maxSpeedPowerUps = 4, double speedPowerUpMultiplier = 80.0);

        // Allow copy
        TickSubSettings(const TickSubSettings&) = default;
        TickSubSettings& operator=(const TickSubSettings&) = default; //cpp20
        // Allow move
        TickSubSettings(TickSubSettings&&)            noexcept = default;
        TickSubSettings& operator=(TickSubSettings&&) noexcept = default; //cpp20
        // Virtual destructor
        virtual ~TickSubSettings() = default;

        //todo: method order matters?
        /// 
        /// \return A rescaled timeVariable (ref?) to current \ref Server's \ref Server::tickRate "tick rate", so it tells the interval in <i>ticks</i> instead of <i>seconds</i>
        /// \pre Expects(tickRate > 0 && tickRate <= 1000);
        /// \pre Expects(timeVariable >= 0.0);
        static int getTickTiedVar(double timeVariable, int tickRate);

        /// \copydoc tickRate_
        /// 
        /// \warning Whenever \ref Server::tickRate "tick rate" of the \ref Server changes, this function should be called as well.
        /// \pre Expects(tickRate > 0 && tickRate <= 1000);
        void setTickRate(int tickRate);
        int getTickRate() const;

        /// 
        /// \return \copybrief speed_
        float getSpeedPerTick() const;
        /// 
        /// \return \copybrief speed_percents_
        double getSpeedPercents() const;
        /// \copybrief speed_percents_
        /// \pre Expects(speed_percents >= 0.0);
        void setSpeedPercents(double speed_percents);

        /// 
        /// \return \copybrief speedRotation_
        float getRotationSpeedPerTick() const;
        /// 
        /// \return \copybrief speedRotation_percents_
        double getRotationSpeedPercents() const;
        /// \copybrief speedRotation_percents_
        /// \pre Expects(speedRotation_percents >= 0.0);
        void setRotationSpeedPercents(double speedRotation_percents);

        /// 
        /// \return \copybrief ticksTimeout_
        int getTimeoutTicks() const;
        /// 
        /// \return \copybrief timeout_seconds_
        double getTimeoutTime() const;
        /// \copydoc timeout_seconds_
        /// 
        /// \pre Expects(timeout_seconds > 0.0);
        void setTimeoutTime(double timeout_seconds);

        /// 
        /// \return \copybrief ticksGrace_
        int getGraceTicks() const;
        /// 
        /// \return \copybrief grace_seconds_
        double getGraceTime() const;
        /// \copydoc grace_seconds_
        /// 
        /// \pre Expects(grace_seconds > 0.0);
        void setGraceTime(double grace_seconds);

        /// 
        /// \return \copybrief ticksFreeze_
        int getFreezeTicks() const;
        /// 
        /// \return \copybrief freeze_seconds_
        double getFreezeTime() const;
        /// \copydoc freeze_seconds_
        /// 
        /// \pre Expects(freeze_seconds > 0.0);
        void setFreezeTime(double freeze_seconds);

        /// 
        /// \return \copybrief ticksWinWait_
        int getWinWaitTicks() const;
        /// 
        /// \return \copybrief winWait_seconds_
        double getWinWaitTime() const;
        /// \copydoc winWait_seconds_
        /// 
        /// \pre Expects(winWait_seconds > 0.0);
        void setWinWaitTime(double winWait_seconds);

        /// Used to check if the \ref Player should start creating a gap in their trail.
        /// 
        /// \note Should be used in expression: `if ((rand() % max) < max * getChancePerTick())`, but with a better pseudorandom number generator and a static_cast.
        /// \return \copybrief tickChance_
        double getGapChancePerTick() const;
        /// 
        /// \return \copybrief chance_percents_
        double getGapChancePercents() const;
        /// \copydoc chance_percents_
        /// 
        /// \pre Expects(chance_percents >= 0.0 && chance_percents <= 100.0);
        void setGapChance(double chance_percent);

        /// 
        /// \return \copybrief ticksMin_
        int getGapMinTicks() const;
        /// 
        /// \return \copybrief min_seconds_
        double getGapMinTime() const;
        /// \copydoc min_seconds_
        /// 
        /// \note if (min_seconds > max_seconds) max_seconds = min_seconds;
        /// \pre Expects(min_seconds >= 0.0);
        void setGapMinTime(double min_seconds);

        /// 
        /// \return \copybrief ticksMax_
        int getGapMaxTicks() const;
        /// 
        /// \return \copybrief max_seconds_          
        double getGapMaxTime() const;
        /// \copydoc max_seconds_
        /// 
        /// \note if (max_seconds < min_seconds) min_seconds = max_seconds;
        /// \pre Expects(max_seconds >= 0.0);
        void setGapMaxTime(double max_seconds);

        ///
        /// \return \copybrief speedPowerUp_seconds_          
        double getSpeedPowerUpTime() const;
        /// \copydoc speedPowerUp_seconds_
        /// 
        /// \pre Expects(speedPowerUp_seconds >= 0.0);
        void setSpeedPowerUpTime(double speedPowerUp_seconds);

        ///
        /// \return \copybrief speedPowerUpMaxLevel_          
        int getSpeedPowerUpMaxLevel() const;
        /// \copydoc speedPowerUpMaxLevel_
        /// 
        /// \pre Expects(maxSpeedPowerUps > 0);
        void setSpeedPowerUpMaxLevel(int maxSpeedPowerUps);

        ///
        /// \return \copybrief speedPowerUpMultiplier_          
        double getSpeedPowerUpMultiplier() const;
        /// \copydoc speedPowerUpMultiplier_
        /// 
        /// \pre Expects(speedPowerUpMultiplier > 0.0);
        void setSpeedPowerUpMultiplier(double speedPowerUpMultiplier);

        /// \copydoc DEFAULT_SAFETY_TICKS_
        /// \param speedPowerup \ref Player's current \ref SpeedPowerUp level.
        int getSafetyTicks(gsl::index speedPowerup = 0);

    protected:
        /// \ref Server's \ref Server::tickRate "tick rate" needed for computation, if any of the other settings changes.
        /// 
        int tickRate_ = 30;

    private:
        /// Adjusts \ref safetyTicksPerSpeedPowerUp_ to new \ref speed_percents_ or \ref SpeedPowerUp settings (\ref speedPowerUp_seconds_ "speedPowerUp_seconds_," \ref speedPowerUpMaxLevel_ "speedPowerUpMaxLevel_," \ref speedPowerUpMultiplier_ "speedPowerUpMultiplier_").
        ///
        void rescaleSafetyTicksPerSpeedPowerUp();

        /// Percentage of the default speed of the \ref Player. Can be above 100%.
        ///
        double speed_percents_ = 100.0;

        /// Percentage of the default rotation speed of the \ref Player. Can be above 100%.
        ///
        double speedRotation_percents_ = 100.0;

        /// The time <i>in seconds</i>, after we fail to receive a packet, the timeout occurs (even if it was an unfortunate series of packet losses and not a lag).
        /// 
        double timeout_seconds_ = 10.0;
        /// The time <i>in seconds</i> before the \ref Player "Players" start drawing their trails.
        /// 
        double grace_seconds_ = 4.0;
        /// The time <i>in seconds</i> the \ref Player "Players" are frozen in motion at the start of the round.
        /// 
        double freeze_seconds_ = 3.0;
        /// The time <i>in seconds</i> the \ref Player "Players" are frozen in motion at the start of the round.
        /// 
        double winWait_seconds_ = 3.0;

        /// The chance per second <i>in percents</i> that the \ref Player will start not drawing their <b>trail</b> for random amount of <i>ticks</i> in range <[ticksMin_];[ticksMax_]>.
        /// 
        double gapChance_percents_ = 39.3;
        /// The <b>minimum</b> time <i>in seconds</i> the \ref Player is supposed to not draw their <b>trail</b>.
        /// 
        double gapMin_seconds_ = 0.33;
        /// The <b>maximum</b> time <i>in seconds</i> the \ref Player is supposed to not draw their <b>trail</b>.
        /// 
        double gapMax_seconds_ = 0.66;
        /// The time <i>in seconds</i> the \ref SpeedPowerUp lasts.
        /// 
        double speedPowerUp_seconds_ = 3.0;

        /// Speed in pixels per second at \ref speed_percents_ equal to 100%.
        ///
        static constexpr float DEFAULT_SPEED_ = 40.f;
        /// Rotation speed in radians per second at \ref speedRotation_percents_ equal to 100%.
        ///
        static constexpr float DEFAULT_ROTATION_SPEED_ = gsl::narrow_cast<float>(M_PI/1.7f);
        /// Lag on collision board, so suicides don't happen.
        ///
        static constexpr float DEFAULT_SAFETY_TICKS_RATIO_ = 0.2f;

        /// \ref Player movement speed in pixels per tick.
        ///
        float speed_ = gsl::narrow_cast<float>(DEFAULT_SPEED_ * speed_percents_/100.0 / tickRate_);

        /// \ref Player rotation speed in radians per tick.
        ///
        float speedRotation_ = gsl::narrow_cast<float>(DEFAULT_ROTATION_SPEED_ * speedRotation_percents_/100.0 / tickRate_);

        /// The amount of <i>ticks</i>, after we fail to receive a packet, the timeout occurs (even if it was an unfortunate series of packet losses and not a lag).
        /// 
        int ticksTimeout_ = qRound(timeout_seconds_ * tickRate_);
        /// The amount of <i>ticks</i> before the \ref Player "Players" start drawing their trails.
        /// 
        int ticksGrace_ = qRound(grace_seconds_ * tickRate_);
        /// The amount of <i>ticks</i> the \ref Player "Players" are frozen in motion at the start of the round.
        /// 
        int ticksFreeze_ = qRound(freeze_seconds_ * tickRate_);
        /// The amount of <i>ticks</i> before the restart of the \ref Game when the winner is already known.
        /// 
        int ticksWinWait_ = qRound(winWait_seconds_ * tickRate_);

        /// The chance per <i>tick</i> (divided by 100, so it represents a ratio and not a percentage) that the \ref Player will start not drawing their <b>trail</b> for a random amount of <i>ticks</i> in range <[ticksMin_];[ticksMax_]>.
        /// 
        double tickGapChance_ = gapChance_percents_ / tickRate_ / 100.0;
        /// The <b>minimum</b> amount of <i>ticks</i> the \ref Player is supposed to not draw their <b>trail</b>.
        /// 
        int ticksGapMin_ = qRound(gapMin_seconds_ * tickRate_);
        /// The <b>maximum</b> amount of <i>ticks</i> the \ref Player is supposed to not draw their <b>trail</b>.
        /// 
        int ticksGapMax_ = qRound(gapMax_seconds_ * tickRate_);
        /// The amount of <i>ticks</i> the \ref SpeedPowerUp lasts.
        ///
        int ticksSpeedPowerUp_ = qRound(speedPowerUp_seconds_ * tickRate_);

        /// Maximum \ref SpeedPowerUp "SpeedPowerUps" that \ref Player can have. Picking up another only refreshes the duration.
        ///
        int speedPowerUpMaxLevel_ = 4;
        /// How much increase in speed one \ref SpeedPowerUp does in percents.
        ///
        double speedPowerUpMultiplier_ = 80.0;
        /// Array of \ref getSafetyTicks(int) "safetyTicks" per \ref SpeedPowerUp level.
        ///
        std::vector<int> safetyTicksPerSpeedPowerUp_;
    };

    /// Settings related to the \ref Game, including network settings.
    ///
    class Settings final : public TickSubSettings
    {
        friend QDataStream& operator<<(QDataStream& stream, const Settings& data);
        friend QDataStream& operator>>(QDataStream& stream, Settings& data);
    public:
        /// \ref Game settings tied with \ref Server::tickRate "tick rate".
        ///
        

        // Constructor
        /// \copydoc TickSubSettings(int,double,double,double,double,double,double,double,double,double);
        /// \param sizeBoard \copybrief sizeBoard_
        /// \param sizeLine \copybrief sizeLine
        /// \param sizePlayer \copybrief sizePlayer
        /// \param wallThickness \copybrief wallThickness
        /// \pre Expects(tickRate > 0 && tickRate <= 1000);
        /// \pre Expects(sizeBoard > ((wallThickness + sizePlayer) * 2 + 1));
        /// \pre Expects(sizeLine > 0)
        /// \pre Expects(sizePlayer > 0);
        /// \pre Expects(wallThickness > 0);
        Settings(int tickRate = 30, int sizeBoard = 512, int sizeLine = 3, int sizePlayer = 6, int wallThickness = 2,
                 double speed_percents = 100.0, double speedRotation_percents = 100.0, double timeout_seconds = 10.0, double grace_seconds = 4.0, double freeze_seconds = 3.0, double winWait_seconds = 3.0, double gapChance_percents = 39.3, double gapMin_seconds = 0.33, double gapMax_seconds = 0.66);

        // Allow copy
        Settings(const Settings&)            = default;
        Settings& operator=(const Settings&) = default; //cpp20
        // Allow move
        Settings(Settings&&)            noexcept = default;
        Settings& operator=(Settings&&) noexcept = default; //cpp20
        // Destructor
        virtual ~Settings() = default;

        /// 
        /// \return \copybrief sizeBoard_
        int getBoardSize() const;
        /// \copydoc sizeBoard_
        /// 
        /// \pre Expects(size > ((wallThickness + sizePlayer) * 2 + 1));
        void setBoardSize(int size);

        /// 
        /// \return \copybrief fontSize_
        int getFontSize() const;

        /// The size of the \ref Player's trail
        /// 
        int sizeLine = 3;
        /// The size of the \ref Player's head
        /// 
        int sizePlayer = 6;

        int wallThickness = 2;

    private:
        /// The board (play area) is always square, so only one variable is needed
        /// 
        int sizeBoard_ = 512;
        int fontSize_  = qRound(sizeBoard_/8.f);
    };
}