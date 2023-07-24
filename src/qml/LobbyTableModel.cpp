#include "qml/LobbyTableModel.h"

int LobbyTableModel::rowCount(const QModelIndex &) const
{
    return 200;
}

int LobbyTableModel::columnCount(const QModelIndex &) const
{
    return 200;
}

QVariant LobbyTableModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return QString("%1, %2").arg(index.column()).arg(index.row());
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> LobbyTableModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}

#include "LobbyTableModel.moc"