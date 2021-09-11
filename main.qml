import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.0 as Platform
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0

Window {
    width: mainLayout.implicitWidth + mainLayout.anchors.margins*2
    height: mainLayout.implicitHeight + mainLayout.anchors.margins*2
    visible: true
    Material.theme: Material.Light
    readonly property real cellSize: 64
    readonly property real cellSizePreview: 16
    title: qsTr("RGB LED Matrix generator")

    Component.onCompleted: {
        lastColorsModel.addColor(colorModel.currentColorIndex)
    }

    Platform.MessageDialog {
        id: confirmDialog
        text: "Are you sure?"
        informativeText: "You want to clear the matrix for current frame"
        buttons: Platform.MessageDialog.Ok|Platform.MessageDialog.Cancel
        onAccepted: colorModel.clearModel()
    }

    Platform.FileDialog {
        id: codeDialog
        title: "Enter file name to save"
        fileMode: Platform.FileDialog.SaveFile
        defaultSuffix: ".h"
        onAccepted: {
            colorModel.generateCppCode(codeDialog.file)
        }
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: 10
        RowLayout {

            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                Grid {
                    id: previewgrid
                    Layout.alignment: Qt.AlignTop
                    width: cellSizePreview*8
                    height: cellSizePreview*8
                    rows: 8
                    columns: 8
                    Repeater {
                        model: colorModel
                        delegate: Item {
                            width: cellSizePreview
                            height: cellSizePreview
                            Rectangle {
                                anchors.fill: parent
                                anchors.margins: 0
                                color: display
                            }
                        }
                    }
                }
                Button {
                    text: "Generate code"
                    Layout.minimumWidth: previewgrid.width
                    onClicked: codeDialog.open()
                }

                Button {
                    text: "Clear"
                    Layout.minimumWidth: previewgrid.width
                    onClicked: confirmDialog.open()
                }
            }

            ColumnLayout {
                id: centerColumn
                Layout.alignment: Qt.AlignCenter
                TabBar {
                    onCurrentIndexChanged: {
                        colorModel.frameIndex = currentIndex
                    }
                    Repeater {
                        model: 5
                        TabButton {
                            text: "Frame " + (index+1)
                        }
                    }
                }

                GridView {
                    id: grid
                    width: cellSize*8
                    height: cellSize*8
                    cellWidth: cellSize
                    cellHeight: cellSize
                    model: colorModel
                    delegate: Item {
                        width: cellSize
                        height: cellSize
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 1
                            color: display
                            radius: 3
                            border.color: "black"
                            border.width: 1
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton | Qt.RightButton
                                onClicked: {
                                    if (mouse.button === Qt.RightButton)
                                        colorModel.setDefaultColor(index)
                                    else
                                        colorModel.setColor(index)
                                }
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.fillHeight: true
                width: (cellSize + 6)*3
                Layout.alignment: Qt.AlignTop
                anchors.margins: 10
                spacing: 5

                Text {
                    text: "Current color:"
                }

                Rectangle {
                    id: currentColorRect
                    width: parent.width
                    height: cellSize
                    radius: 3
                    color: colorModel.currentColor
                    border.color: "black"
                    border.width: 1
                    layer.enabled: true
                    layer.effect: DropShadow {
                        transparentBorder: true
                        color: "#000000"
                        spread: 0.5
                        radius: 3
                        horizontalOffset: 4
                        verticalOffset: 4
                    }
                }

                Text {
                    text: "Last selected colors:"
                }

                GridView {
                    width: parent.width
                    height: parent.width
                    model: lastColorsModel
                    cellWidth: cellSize + 6
                    cellHeight: cellSize + 6
                    delegate: Item {
                        width: cellSize + 6
                        height: cellSize + 6
                        Rectangle {
                            anchors.centerIn: parent
                            width: cellSize
                            height: cellSize
                            color: display
                            radius: 3
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    colorModel.currentColorIndex = lastColorsModel.getColorIndex(index)
                                }
                            }
                            layer.enabled: true
                            layer.effect: DropShadow {
                                transparentBorder: true
                                color: "#000000"
                                spread: 0.5
                                radius: 3
                                horizontalOffset: 2
                                verticalOffset: 2
                            }
                        }
                    }
                }
            }
        }

        GridView {
            Layout.fillWidth: true
            height: cellSize
            flow: GridView.FlowTopToBottom
            model: 256
            clip: true
            cellWidth: cellSize/2
            cellHeight: cellSize/2
            delegate: Rectangle {
                width: cellSize/2
                height: cellSize/2
                color: colorModel.matrixColor(index)
                border.color: "black"
                border.width: 1
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        colorModel.currentColorIndex = index
                        lastColorsModel.addColor(index)
                    }
                }
            }
        }
    }
}
