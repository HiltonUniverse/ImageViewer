import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Rectangle
{
    id: editSubActions

    color: "#828282"

    signal freeHandStarted
    signal colorPenSelected(var color)
    RowLayout
    {
        anchors.fill: parent
        spacing: 0

        Action
        {
            id: openColorAction
            icon.name: "edit-color-dialog"
            icon.source: "qrc:/images/color-palate.svg"
            onTriggered: penColorDialog.open()
        }

        //select color dialog
        ToolButton
        {
            id: openColorActionButton
            action: openColorAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color: openColorActionButton.hovered ? "#b7b7b7" : "transparent"
                radius: 2
            }

            ToolTip
            {
                visible: openColorActionButton.hovered
                text: qsTr("Select pen color")

                background: Rectangle
                {
                    color: Qt.rgba(0.74, 0.74, 0.74, 0.7)
                    anchors.fill: parent
                    radius: 2
                }
            }
        }


        ColorDialog
        {
            id: penColorDialog
            currentColor: "red"
            onAccepted:
            {
                editSubActions.colorPenSelected(penColorDialog.color)
            }
        }

        //Free Handl Drawing
        Action
        {
            id: freeHandlDrawingAction
            icon.name: "edit-freehand"
            icon.source: "qrc:/images/free_hand.svg"
            onTriggered: freeHandStarted()
        }

        ToolButton
        {
            id: freeHandlDrawingButton
            action: freeHandlDrawingAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color: freeHandlDrawingButton.hovered ? "#b7b7b7" : "transparent"
                radius: 2
            }

            ToolTip
            {
                visible: freeHandlDrawingButton.hovered
                text: qsTr("Free hand drawing")

                background: Rectangle
                {
                    color: Qt.rgba(0.74, 0.74, 0.74, 0.7)
                    anchors.fill: parent
                    radius: 2
                }
            }
        }

        ToolSeparator
        {
            implicitHeight: 20
            implicitWidth: 10
        }

        //Convert image to grayscale
        Action
        {
            id: convertGrayscaleAction
            icon.name: "edit-freehand"
            icon.source: "qrc:/images/grayscale_icon.svg"
            onTriggered: cpp_image_processing_handler.convertToGrayscale()
        }

        ToolButton
        {
            id: convertGrayscaleButton
            action: convertGrayscaleAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color: convertGrayscaleButton.hovered ? "#b7b7b7" : "transparent"
                radius: 2
            }

            ToolTip
            {
                visible: convertGrayscaleButton.hovered
                text: qsTr("GrayScale")

                background: Rectangle
                {
                    color: Qt.rgba(0.74, 0.74, 0.74, 0.7)
                    anchors.fill: parent
                    radius: 2
                }
            }
        }

        //Convert image to binary
        Action
        {
            id: convertBinaryAction
            icon.name: "edit-freehand"
            icon.source: "qrc:/images/binary.svg"
            onTriggered: cpp_image_processing_handler.convertToBinary()
        }

        ToolButton
        {
            id: convertBinaryButton
            action: convertBinaryAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color: convertBinaryButton.hovered ? "#b7b7b7" : "transparent"
                radius: 2
            }

            ToolTip
            {
                visible: convertBinaryButton.hovered
                text: qsTr("Binary")

                background: Rectangle
                {
                    color: Qt.rgba(0.74, 0.74, 0.74, 0.7)
                    anchors.fill: parent
                    radius: 2
                }
            }
        }

        //Convert image to inverse binary
        Action
        {
            id: convertInverseBinaryAction
            icon.name: "edit-freehand"
            icon.source: "qrc:/images/inverse_binary.svg"
            onTriggered: cpp_image_processing_handler.convertToInverseBinary()
        }

        ToolButton
        {
            id: convertInverseBinaryButton
            action: convertInverseBinaryAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color: convertInverseBinaryButton.hovered ? "#b7b7b7" : "transparent"
                radius: 2
            }

            ToolTip
            {
                visible: convertInverseBinaryButton.hovered
                text: qsTr("Inverse Binary")

                background: Rectangle
                {
                    color: Qt.rgba(0.74, 0.74, 0.74, 0.7)
                    anchors.fill: parent
                    radius: 2
                }
            }
        }

        ToolSeparator
        {
            implicitHeight: 20
            implicitWidth: 10
        }

        //wrap action
        Action
        {
            id: wrapImageAction
            icon.name: "edit-freehand"
            icon.source: "qrc:/images/warp.svg"
            onTriggered: cpp_image_processing_handler.wrapActiveImage()
        }

        ToolButton
        {
            id: wrapImageButton
            action: wrapImageAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color: wrapImageButton.hovered ? "#b7b7b7" : "transparent"
                radius: 2
            }

            ToolTip
            {
                visible: wrapImageButton.hovered
                text: qsTr("Wrap")

                background: Rectangle
                {
                    color: Qt.rgba(0.74, 0.74, 0.74, 0.7)
                    anchors.fill: parent
                    radius: 2
                }
            }
        }

        Item
        {
            Layout.fillWidth: true
        }
    }
}
