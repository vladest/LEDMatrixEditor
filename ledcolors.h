#ifndef LEDCOLORS_H
#define LEDCOLORS_H

#include <QColor>
#include <QObject>
#include <QAbstractItemModel>

constexpr int sFramesNumber = 5;
constexpr quint8 sBlack = 0xFF;
constexpr quint8 sWhite = 0xFE;

class LEDColors : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QColor currentColor READ currentColor NOTIFY currentColorChanged)
    Q_PROPERTY(int frameIndex READ frameIndex WRITE setFrameIndex NOTIFY frameIndexChanged)
    Q_PROPERTY(int currentColorIndex READ currentColorIndex WRITE setCurrentColorIndex NOTIFY currentColorIndexChanged)
    //Q_PROPERTY(QList<QColor> matrixColors READ matrixColors CONSTANT)
public:
    explicit LEDColors(QObject *parent = nullptr);
    virtual ~LEDColors();
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int columnCount(const QModelIndex &parent) const override;

    QColor currentColor() const;
    int frameIndex() const;
    int currentColorIndex() const;

    Q_INVOKABLE void setCurrentColorIndex(int colorIndex);
    Q_INVOKABLE void setFrameIndex(int frameIndex);
    Q_INVOKABLE void setColor(int modelIndex);
    Q_INVOKABLE void setDefaultColor(int modelIndex);
    static Q_INVOKABLE QColor matrixColor(int index);
    Q_INVOKABLE void clearModel();
    Q_INVOKABLE bool generateCppCode(const QUrl &fileName);

signals:
    void currentColorChanged();
    void frameIndexChanged();
    void currentColorIndexChanged();

private:
    bool frameIsEmpty(int modelIndex);

private:
    int m_currentColorIndex = sWhite; // white
    QVector<quint8> m_colors[sFramesNumber];
    static QList<QColor> m_matrixLedcolors;
    int m_frameIndex = 0;
};

#endif // LEDCOLORS_H
