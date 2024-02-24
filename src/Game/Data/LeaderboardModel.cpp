#include "Game/Data/LeaderboardModel.h"

LeaderboardModel::LeaderboardModel(QObject* parent)
	: QAbstractListModel(parent)
{
}

void LeaderboardModel::addPlayer(const Player& player)
{
    playerScores.emplaceBack(player.name, player.score, player.getColor());
}

QHash<int, QByteArray> LeaderboardModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(Roles::name)]      = "name";
    roles[static_cast<int>(Roles::score)]     = "score";
    roles[static_cast<int>(Roles::textColor)] = "textColor";
    return roles;
}