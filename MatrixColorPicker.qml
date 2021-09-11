import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    id: dialog
    title: "Please pickup the color"
    standardButtons: Dialog.Cancel
    property int selectedColorIndex
    modal: true
    readonly property real cellSize: 32
    GridView {
        implicitWidth: cellSize*8
        implicitHeight: cellSize*8
        cellWidth: cellSize
        cellHeight: cellSize
        model: 256

        delegate: Rectangle {
            width: cellSize
            height: cellSize
            color: colorModel.matrixColor(index)
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    selectedColorIndex = index
                    accept()
                }
            }
        }
    }
}

