#pragma once

#include <QColor>
#include <QObject>
#include <QAbstractItemModel>

class LastColors : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit LastColors(QObject *parent = nullptr);
    virtual ~LastColors();
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int columnCount(const QModelIndex &parent) const override;

    Q_INVOKABLE void addColor(int colorIndex);
    Q_INVOKABLE int  getColorIndex(int modelIndex);

private:
    QVector<quint8> m_lastColors;
};

