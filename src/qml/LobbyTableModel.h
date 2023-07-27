#pragma once
#include <qqml.h>
#include <QAbstractTableModel>

class LobbyTableModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;
};