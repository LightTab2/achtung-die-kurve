#pragma once

#include <QColor>
#include <QDataStream>
#include <QPainter>
#include <QPoint>
#include <QString>

#include "Game/Data/Settings/GameSettings.h"
#include "Helpers/WrappingData.h"

/// Represents a single snake-like \ref Player in the game.
///
class Player final
{
    friend QDataStream& operator<<(QDataStream& stream, const Player& data);
    friend QDataStream& operator>>(QDataStream& stream, Player& data);
public:
    enum class Direction : bool
    {
        left  = false,
        right = true
    };
    /// A line drawn by a \ref Player in a single tick.
    ///
    struct DrawData
    {
        friend QDataStream& operator<<(QDataStream& stream, const DrawData& data);
        friend QDataStream& operator>>(QDataStream& stream, DrawData& data);

        // Constructor
        DrawData() = default;
        DrawData(QPointF lineEnd, bool bNoDraw = false);

        QPointF lineEnd{ 0.f, 0.f };

        /// Whether this DrawData should draw a line on the board.
        /// 
        /// The packet cannot be omitted instead, because it would be impossible to tell if we acquired all of the packets
        /// and there would be no way of telling where the head of the \ref Player is.
        bool bNoDraw = false;
    };

    /// A single packet that informs about change in \ref Player's movement.
    ///
    struct MovementData
    {
        friend QDataStream& operator<<(QDataStream& stream, const MovementData& data);
        friend QDataStream& operator>>(QDataStream& stream, MovementData& data);

        // Constructor
        MovementData() = default;
        MovementData(DrawData drawData, float angle, bool bDemise = false, int noDrawTicks = -1);

        DrawData drawData{};

        /// Needed for \ref Player's movement prediction during a lag.
        ///
        float angle = 0.f;

        bool bDemise = false;

        std::optional<int> noDrawTicks;
    };

    // Constructor
    Player(const QString& name = "", bool bLocal = false);
    // Allow copy
    Player(const Player& obj)             = default;    
    Player& operator=(const Player& rhs)  = default;
    // Allow move
    Player(Player&& obj)        noexcept = default;
    Player& operator=(Player&&) noexcept = default;
    // Virtual destructor
    virtual ~Player() = default;

    // Equality operators
    bool operator==(const QString& name) const;
    bool operator==(const Player& obj)   const = default;
    bool operator!=(const QString& name) const;
    bool operator!=(const Player& obj)   const = default;
    operator bool() const;

    /// Resets the \ref Player, so it's ready for a new game.
    /// 
    void reset(gsl::not_null<const Game::Settings*> settings) noexcept;

    ///
    /// \param drawIndex Time (<i>tick</i>) represented by an index in \ref data_. If the \ref Player is dead, it is ignored and \ref deadPos_ is used instead.
    void drawHead(QPainter* painter, const WrappingData<int>& drawIndex, int playerSize) const;
    ///
    /// \param drawIndex Time (<i>tick</i>) represented by an index in \ref data_.
    void drawLine(QPainter* painter, const WrappingData<int>& drawIndex, const QColor& color, int lineSize, Qt::PenCapStyle capStyle = Qt::SquareCap) const;

    //todo: check index ref
    /// Calculates next position from previous <i>tick</i> to target (\ref index) tick.
    ///
    void calculateNextMove(const WrappingData<int>& index, float speed, float rotationSpeed);

    /// 
    /// \param drawIndex Time (<i>tick</i>) represented by an index in \ref data_.
    void checkWallCollision(const WrappingData<int>& index, int boardSize, int wallThickness);
    void die(gsl::index index);

    /// Color differs if the \ref Player is dead.
    ///
    QColor getColor() const;
    ///
    /// \pre Expects(colorIndex >= 0 && colorIndex < 8);
    void setColorIndex(int colorIndex);

    void startRotating(Direction direction);
    void stopRotating(Direction direction);

    void setNoDrawTicks(int noDrawTicks);

    QString name = "";

    /// Whether the \ref Player is already in the game or has just joined.
    ///
    bool bActive = false;

    /// Whether the \ref Player belongs to current client/server and they move them.
    bool bLocal = false;

    /// An unique token to allow smooth reconnects and forbid stealing indentity.
    /// 
    int token = -1;

    /// Cumulative amount of beaten \ref Player "Players". 
    ///
    /// A \ref Player is beaten by another if they survive longer.
    int score = 0;

    Qt::Key left  = Qt::Key_A;
    Qt::Key right = Qt::Key_D;

    static constexpr QColor playerColors[8]
    { 
        QColor{ 225, 0,   0   }, //red
        QColor{ 1,   225, 0   }, //green, 1 red for easier collision checking
        QColor{ 225, 225, 0   }, //yellow
        QColor{ 225, 0,   225 }, //pink
        QColor{ 225, 125, 0  },  //orange
        QColor{ 125, 0,   225 }, //purple 
        QColor{ 225, 225, 225 }, //white
        QColor{ 105, 105, 105 }  //grey
    };

    static constexpr int darken = 120;

    static constexpr QColor playerDeadColors[8]
    {
        QColor{ 225 - darken,     0           ,     0                }, //dead red
        QColor{ 0,                225 - darken,     0                }, //dead green
        QColor{ 225 - darken,     225 - darken,     0                }, //dead yellow
        QColor{ 225 - darken,     0           ,     225 - darken     }, //dead pink
        QColor{ 225 - darken,     125 - darken / 2, 0                }, //dead brown
        QColor{ 125 - darken / 2, 0,                225 - darken     }, //dead purple 
        QColor{ 225 - darken,     225 - darken,     225 - darken     }, //dead white
        QColor{ 105 - darken / 2, 105 - darken / 2, 105 - darken / 2 }  //dead grey
    };

private:
    enum class Input : quint8
    {
        noMovement  = 0b00,
        left        = 0b01,
        right       = 0b10,
        noMovement2 = 0b11
    };

    bool isValid() const;

    bool bDead_ = false;

    /// After the \ref Player has died, they might have not sent all their movementData to others
    /// and need to redraw the wrongly predicted lines.
    ///
    bool bAllPacketsDelivered_ = false;

    /// Cached position at death.
    /// 
    QPointF deadPos_{ 0.f, 0.f };

    int colorIndex_ = 0;
    
    //add reference to Input class?
    /// Whether the \ref Player is moving and in which direction.
    /// 
    /// 0b00 - no movement
    /// 0b01 - left
    /// 0b10 - right
    /// 0b11 - no movement 
    /// \todo format this description as a list
    quint8 input_ = 0u;

    std::vector<MovementData> data_{};
    
    /// In this game it is important that there are no lost packets and ask for retransmission if it happens. This vector tracks if packets were delivered. 
    std::vector<bool> confirm_{};

    /// Number of ticks when they do not draw their tail, creating a gap.
    int noDrawTicks_ = 0;
};