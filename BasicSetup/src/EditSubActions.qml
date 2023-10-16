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
    signal colorBackgroundSelected(var color)

    RowLayout
    {
        anchors.fill: parent
        spacing: 0


        //Change background action
        Action
        {
            id: changeBackgroundAction
            icon.source: "file:///Users/Hilton/Desktop/background.svg"
            onTriggered: backgroundColorDialog.open()
        }

        //select color dialog
        ToolButton
        {
            id: changeBackgroundActionButton
            action: changeBackgroundAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color: changeBackgroundActionButton.hovered ? "#b7b7b7" : "transparent"
                radius: 2
            }

            ToolTip
            {
                visible: changeBackgroundActionButton.hovered
                text: qsTr("Select background")

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
            id: backgroundColorDialog
            currentColor: "red"
            onAccepted:
            {
                editSubActions.colorBackgroundSelected(backgroundColorDialog.color)
            }
        }

        Action
        {
            id: openColorAction
            icon.name: "edit-color-dialog"
            icon.source: "file:///Users/Hilton/Desktop/color-palate.svg"
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
            icon.source: "file:///Users/Hilton/Desktop/free_hand.svg"
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

        Item
        {
            Layout.fillWidth: true
        }
    }
}
