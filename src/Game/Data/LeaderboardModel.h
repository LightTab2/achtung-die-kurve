#pragma once
#include <QAbstractListModel>

#include <QVariantList>

#include "Game/Data/Player/Player.h"

class LeaderboardModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum class Roles : int
    {
        name = Qt::UserRole + 1,
        score,
        textColor
    };

    struct PlayerScore
    {
        QString name{};
        int score = 0;
        QColor color{};
    };
    // Constructor
    LeaderboardModel(QObject* parent = nullptr);
    // Disallow copy
    LeaderboardModel(const LeaderboardModel& obj)            = delete;
    LeaderboardModel& operator=(const LeaderboardModel& rhs) = delete;
    // Allow move
    LeaderboardModel(LeaderboardModel&& obj)        noexcept = default;
    LeaderboardModel& operator=(LeaderboardModel&&) noexcept = default;
    // Virtual destructor
    virtual ~LeaderboardModel() = default;

    void addPlayer(const Player& player);

protected:
    QHash<int, QByteArray> roleNames() const override;

    Qt::ItemFlags flags(const QModelIndex&) const
    {
        return Qt::ItemNeverHasChildren;
    }

    int rowCount(const QModelIndex& parent) const override
    {
        return playerScores.size();
    }

    QVariant data(const QModelIndex& index, int role) const override
    {
        switch (static_cast<Roles>(role))
        {
        case Roles::name:
            return playerScores[index.row()].name;
            break;
        case Roles::score:
            return playerScores[index.row()].score;
            break;
        case Roles::textColor:
            return playerScores[index.row()].color;
            break;
        default:
            break;
        }

        return QVariant();
    }

private:
    QVector<PlayerScore> playerScores{ {"QWERTYPqqwertep,,", 12, Qt::red}, {"MMMMMMMMMMMMMMMM", 1, Qt::blue} };
};