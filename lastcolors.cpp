#include "lastcolors.h"
#include "ledcolors.h"

#include <QDataStream>
#include <QFile>

LastColors::LastColors(QObject *parent) : QAbstractItemModel(parent)
{
    QFile file("lastcolors.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in >> m_lastColors;
        file.close();
    }
}

LastColors::~LastColors()
{
    QFile file("lastcolors.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << m_lastColors;
    file.close();
}

int LastColors::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_lastColors.size();
}

QVariant LastColors::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    if (!index.isValid() || index.row() >= 64)
        return QVariant();
    return LEDColors::matrixColor(m_lastColors.at(index.row()));
}

QModelIndex LastColors::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column, nullptr);
}

QModelIndex LastColors::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int LastColors::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

void LastColors::addColor(int colorIndex)
{
    if (m_lastColors.contains(colorIndex))
        return;
    beginInsertRows(QModelIndex(), m_lastColors.size(), m_lastColors.size());
    m_lastColors.append(colorIndex);
    endInsertRows();
}

int LastColors::getColorIndex(int modelIndex)
{
    return m_lastColors.at(modelIndex);
}
