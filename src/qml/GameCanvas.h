#pragma once
#include <qqml.h>
#include <QContext2D>

class GameCanvas : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;
};