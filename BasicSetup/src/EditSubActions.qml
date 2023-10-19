import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Rectangle
{
    id: editSubActions

    color: "#828282"

    signal freeHandStarted
    signal addTextStarted
    signal colorPenSelected(var color)
    RowLayout
    {
        anchors.fill: parent
        spacing: 0

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

        ToolSeparator
        {
            implicitHeight: 20
            implicitWidth: 10
        }

        //Add text
        Action
        {
            id: addTextAction
            icon.name: "edit-freehand"
            icon.source: "file:///Users/Hilton/Desktop/add_text.svg"
            onTriggered: addTextStarted()
        }

        ToolButton
        {
            id: addTextButton
            action: addTextAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color: addTextButton.hovered ? "#b7b7b7" : "transparent"
                radius: 2
            }

            ToolTip
            {
                visible: addTextButton.hovered
                text: qsTr("Add text")

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
