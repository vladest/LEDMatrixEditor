#include "ledcolors.h"

#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QUrl>

QColor colorInterpolate(const QColor& start, const QColor& end, int colors, int index)
{
    float ratio = 1.0f/colors*float(index);
    int r = (int)(ratio*start.red() + (1-ratio)*end.red());
    int g = (int)(ratio*start.green() + (1-ratio)*end.green());
    int b = (int)(ratio*start.blue() + (1-ratio)*end.blue());
    return QColor::fromRgb(r,g,b);
}

// assigned 256 colors for the LED matrix
static const QList<QPair<QColor, QColor>> m_rgbLEDColors = {
    {{185,   0,   0}, {178,   0,   0}},
    {{178,   0,   1}, {178,  55,  15}},
    {{180,  64,  18}, {175, 120,  45}},
    {{175, 125,  50}, {135, 170,  65}},
    {{150, 185,  80}, {105, 205, 115}},
    {{105, 205, 115}, { 40, 205, 155}},
    {{  0, 205, 105}, {  0, 205, 160}},
    {{  0, 205, 165}, {  0, 205, 190}},

    {{  0, 205,  75}, {  0, 205,  95}},
    {{  0, 205, 115}, {  0, 205, 135}},
    {{  0, 205, 140}, {  0, 205, 150}},
    {{  0, 205, 155}, {  0, 205, 175}},
    {{  0, 205, 195}, {  0, 200, 205}},
    {{  0, 200, 190}, {  0, 200, 205}},
    {{  0, 200, 205}, {  0, 195, 205}},
    {{  0, 195, 205}, {  0, 197, 206}},

    {{  0, 195, 205}, {  0, 185, 205}},
    {{  0, 190, 205}, {  0, 165, 205}},
    {{  0, 167, 205}, {  0, 115, 205}},
    {{  0, 117, 205}, {  0,  85, 205}},
    {{  0,  70, 205}, {  0,  65, 205}},
    {{  0,  45, 205}, {  0,  70, 205}},
    {{  0,  65, 205}, {  0,  65, 205}},
    {{  0,  65, 205}, {  0,  55, 205}},

    // only 7 lines here
    {{  0,  60, 205}, { 50,  60, 205}},
    {{ 60,  85, 205}, {120,  75, 205}},
    {{135,  85, 205}, {155,  65, 205}},
    {{160,  35, 205}, {155,  15, 205}},
    {{165,  15, 210}, {180,  15, 205}},
    {{195,  15, 205}, {180,  10, 160}},
    {{187,  10, 150}, { 185,  0,   5}},
};

static const QList<QColor> m_rgbLEDColorsLastLine = {
    {186, 0, 5}, {189, 0, 7}, {192, 0, 9}, {195, 0, 10}, {197, 0, 10}, {200, 0, 11}, {255, 255, 255}, {0, 0, 0}
};

QList<QColor> LEDColors::m_matrixLedcolors = QList<QColor>();


LEDColors::LEDColors(QObject *parent) : QAbstractItemModel(parent)
{
    //fillup matrix led colors
    for (const auto& colorPair :  m_rgbLEDColors) {
        for (int i = 0; i < 8; i++) {
            const auto color = colorInterpolate(colorPair.first, colorPair.second, 8, i);
            m_matrixLedcolors.append(color);
        }
    }
    m_matrixLedcolors.append(m_rgbLEDColorsLastLine);

    for (int i = 0; i < sFramesNumber; i++) {
        m_colors[i].fill(sBlack, 64);
    }

    QFile file("currentmodel.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        for (int i = 0; i < sFramesNumber; i++) {
            in >> m_colors[i];
        }
        file.close();
    }
}

LEDColors::~LEDColors()
{
    QFile file("currentmodel.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    for (int i = 0; i < sFramesNumber; i++) {
        out << m_colors[i];
    }
    file.close();
}

int LEDColors::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 64;
}

QVariant LEDColors::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    if (!index.isValid() || index.row() >= 64)
        return QVariant();
    return matrixColor(m_colors[m_frameIndex].at(index.row()));
}

QModelIndex LEDColors::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column, nullptr);
}

QModelIndex LEDColors::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int LEDColors::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QColor LEDColors::currentColor() const
{
    return matrixColor(m_currentColorIndex);
}

void LEDColors::setCurrentColorIndex(int colorIndex)
{
    if (m_currentColorIndex == colorIndex)
        return;
    m_currentColorIndex = colorIndex;
    emit currentColorChanged();
}

void LEDColors::setFrameIndex(int frameIndex)
{
    m_frameIndex = frameIndex;
    if (m_frameIndex < 0) {
        m_frameIndex = 0;
    } else if (m_frameIndex > 4) {
        m_frameIndex = 4;
    }
    emit frameIndexChanged();
    beginResetModel();
    endResetModel();
}

void LEDColors::setColor(int modelIndex)
{
    if (modelIndex < 0 || modelIndex >= 64)
        return;
    m_colors[m_frameIndex][modelIndex] = m_currentColorIndex;
    emit dataChanged(index(modelIndex, 0, QModelIndex()), index(modelIndex, 0, QModelIndex()));
}

void LEDColors::setDefaultColor(int modelIndex)
{
    if (modelIndex < 0 || modelIndex >= 64)
        return;
    m_colors[m_frameIndex][modelIndex] = sBlack;
    emit dataChanged(index(modelIndex, 0, QModelIndex()), index(modelIndex, 0, QModelIndex()));

}

QColor LEDColors::matrixColor(int index)
{
    if (index < 0 || index >= m_matrixLedcolors.size()) {
        qWarning() << "invalid index" << index << "of" << m_matrixLedcolors.size();
        return QColor();
    }
    return m_matrixLedcolors.at(index);
}

void LEDColors::clearModel()
{
    beginResetModel();
    m_colors[m_frameIndex].fill(sBlack, 64);
    endResetModel();
}

bool LEDColors::frameIsEmpty(int frameIndex)
{
    if (frameIndex < 0 || frameIndex >= sFramesNumber)
        return true;
    for (int j = 0; j < m_colors[frameIndex].size(); j++) {
        if (m_colors[frameIndex][j] != sBlack) {
            return false;
        }
    }
    return true;
}

bool LEDColors::generateCppCode(const QUrl &fileName)
{
    QFile data(fileName.toLocalFile());
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        out.setIntegerBase(16);
        out.setNumberFlags(QTextStream::UppercaseDigits);
        out << "uint8_t led_frames[] = {\n";
        for (int i = 0; i < sFramesNumber; i++) {
            if (frameIsEmpty(i)) {
                out << "    // the following frame is empty\n";
            }
            out << "    ";
            for (int j = 0; j < m_colors[i].size(); j++) {
                out << "0x";
                out.setFieldWidth(2);
                out.setFieldAlignment(QTextStream::AlignRight);
                out.setPadChar('0');
                out << m_colors[i][j];
                out.setFieldWidth(0);
                out.setPadChar(' ');
                out << ", ";
                if (j > 0 && ((j + 1) % 8 == 0)) {
                    out << "\n    ";
                }
            }
            out << "\n";
        }
        out << "};";
    } else {
        qWarning() << "Error opening file" << fileName.toDisplayString();
        return false;
    }

    return true;
}

int LEDColors::frameIndex() const
{
    return m_frameIndex;
}

int LEDColors::currentColorIndex() const
{
    return m_currentColorIndex;
}
