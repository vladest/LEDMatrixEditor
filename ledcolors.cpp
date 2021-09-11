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

QList<QColor> LEDColors::m_matrixLedcolors = {
    "#ff0000", "#ff0600", "#ff0c00", "#ff1200", "#ff1800", "#ff1e00", "#ff2400", "#ff2a00",
    "#ff3000", "#ff3600", "#ff3c00", "#ff4200", "#ff4800", "#ff4e00", "#ff5400", "#ff5a00",
    "#ff6000", "#ff6600", "#ff6c00", "#ff7200", "#ff7800", "#ff7e00", "#ff8400", "#ff8a00",
    "#ff9000", "#ff9600", "#ff9c00", "#ffa200", "#ffa800", "#ffae00", "#ffb400", "#ffba00",
    "#ffc000", "#ffc600", "#ffcc00", "#ffd200", "#ffd800", "#ffde00", "#ffe400", "#ffea00",
    "#fff000", "#fff600", "#ffff00", "#f9ff00", "#f3ff00", "#edff00", "#e7ff00", "#e1ff00",
    "#dbff00", "#d5ff00", "#cfff00", "#c9ff00", "#c3ff00", "#bdff00", "#b7ff00", "#b1ff00",
    "#abff00", "#a5ff00", "#9fff00", "#99ff00", "#93ff00", "#8dff00", "#87ff00", "#81ff00",
    "#7bff00", "#75ff00", "#6fff00", "#69ff00", "#63ff00", "#5dff00", "#57ff00", "#51ff00",
    "#4bff00", "#45ff00", "#3fff00", "#39ff00", "#33ff00", "#2dff00", "#27ff00", "#21ff00",
    "#1bff00", "#15ff00", "#0fff00", "#09ff00", "#03ff00", "#00ff00", "#00ff06", "#00ff0c",
    "#00ff12", "#00ff18", "#00ff1e", "#00ff24", "#00ff2a", "#00ff30", "#00ff36", "#00ff3c",
    "#00ff42", "#00ff48", "#00ff4e", "#00ff54", "#00ff5a", "#00ff60", "#00ff66", "#00ff6c",
    "#00ff72", "#00ff78", "#00ff7e", "#00ff84", "#00ff8a", "#00ff90", "#00ff96", "#00ff9c",
    "#00ffa2", "#00ffa8", "#00ffae", "#00ffb4", "#00ffba", "#00ffc0", "#00ffc6", "#00ffcc",
    "#00ffd2", "#00ffd8", "#00ffde", "#00ffe4", "#00ffea", "#00fff0", "#00fff6", "#00fffc",
    "#00ffff", "#00f9ff", "#00f3ff", "#00edff", "#00e7ff", "#00e1ff", "#00dbff", "#00d5ff",
    "#00cfff", "#00c9ff", "#00c3ff", "#00bdff", "#00b7ff", "#00b1ff", "#00abff", "#00a5ff",
    "#009fff", "#0099ff", "#0093ff", "#008dff", "#0087ff", "#0081ff", "#007bff", "#0075ff",
    "#006fff", "#0069ff", "#0063ff", "#005dff", "#0057ff", "#0051ff", "#004bff", "#0045ff",
    "#003fff", "#0039ff", "#0033ff", "#002dff", "#0027ff", "#0021ff", "#001bff", "#0015ff",
    "#000fff", "#0009ff", "#0000ff", "#0600ff", "#0c00ff", "#1200ff", "#1800ff", "#1e00ff",
    "#2400ff", "#2a00ff", "#3000ff", "#3600ff", "#3c00ff", "#4200ff", "#4800ff", "#4e00ff",
    "#5400ff", "#5a00ff", "#6000ff", "#6600ff", "#6c00ff", "#7200ff", "#7800ff", "#7e00ff",
    "#8400ff", "#8a00ff", "#9000ff", "#9600ff", "#9c00ff", "#a200ff", "#a800ff", "#ae00ff",
    "#b400ff", "#ba00ff", "#c000ff", "#c600ff", "#cc00ff", "#d200ff", "#d800ff", "#de00ff",
    "#e400ff", "#ea00ff", "#f000ff", "#f600ff", "#ff00ff", "#ff00f9", "#ff00f3", "#ff00ed",
    "#ff00e7", "#ff00e1", "#ff00db", "#ff00d5", "#ff00cf", "#ff00c9", "#ff00c3", "#ff00bd",
    "#ff00b7", "#ff00b1", "#ff00ab", "#ff00a5", "#ff009f", "#ff0099", "#ff0093", "#ff008d",
    "#ff0087", "#ff0081", "#ff007b", "#ff0075", "#ff006f", "#ff0069", "#ff0063", "#ff005d",
    "#ff0057", "#ff0051", "#ff004b", "#ff0045", "#ff003f", "#ff0039", "#ff0033", "#ff002d",
    "#ff0027", "#ff0021", "#ff001b", "#ff0015", "#ff000f", "#ff0009", "#ffffff", "#000000"
};


LEDColors::LEDColors(QObject *parent) : QAbstractItemModel(parent)
{
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
