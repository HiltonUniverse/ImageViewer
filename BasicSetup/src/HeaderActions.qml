import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar
{
    signal editClicked
    signal shareClicked

    property bool edit_subaction_selected: false
    property bool share_subaction_selected: false

    background: Rectangle
    {
        implicitWidth: 100
        implicitHeight: 20
        opacity: enabled ? 1 : 0.3
        color: "#919191"
    }

    RowLayout
    {
        anchors.fill: parent
        spacing: 0

        //save
        Action
        {
            id: saveAction
            icon.name: "edit-save"
            icon.source: "file:///Users/Hilton/Desktop/save.svg"
            shortcut: StandardKey.Save
            onTriggered: cpp_save_handler.start()
        }

        ToolButton
        {
            id: saveToolButton

            action: saveAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color: saveToolButton.hovered ? "#b7b7b7" : "transparent"
                radius: 2
            }

            ToolTip
            {
                visible: saveToolButton.hovered
                text: qsTr("Save")

                background: Rectangle
                {
                    color: Qt.rgba(0.74, 0.74, 0.74, 0.7)
                    anchors.fill: parent
                    radius: 2
                }
            }
        }

        //UNDO
        Action
        {
            id: undoAction
            icon.name: "edit-undo"
            icon.source: "file:///Users/Hilton/Desktop/undo.svg"
            shortcut: StandardKey.Undo
            onTriggered: cpp_datamanager.undo();
        }

        ToolButton
        {
            id: undoToolButton

            action: undoAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color: undoToolButton.hovered ? "#b7b7b7" : "transparent"
                radius: 2
            }

            ToolTip
            {
                visible: undoToolButton.hovered
                text: qsTr("Undo")

                background: Rectangle
                {
                    color: Qt.rgba(0.74, 0.74, 0.74, 0.7)
                    anchors.fill: parent
                    radius: 2
                }
            }
        }

        //REDO
        Action
        {
            id: redoAction
            icon.name: "edit-redo"
            icon.source: "file:///Users/Hilton/Desktop/redo.svg"
            shortcut: StandardKey.Redo
            onTriggered: cpp_datamanager.redo();
        }

        ToolButton
        {
            id: redoToolButton

            action: redoAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color:
                {
                    if(enabled)
                    {
                        redoToolButton.hovered ? "#b7b7b7" : "transparent"
                    }
                    else
                    {
                        return "red"
                    }
                }
                radius: 2
            }

            ToolTip
            {
                visible: redoToolButton.hovered
                text: qsTr("Redo")

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

        //Edit
        ToolButton
        {
            id: editActionButton

            contentItem: Label
            {
                text: "Edit"
                color: "black"
            }

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color:
                {
                    if(edit_subaction_selected)
                    {
                        return "#b7b7b7"
                    }

                    return editActionButton.hovered ? "#b7b7b7" : "transparent"
                }
                radius: 2
            }

            onClicked:
            {
                editClicked();
                edit_subaction_selected = !edit_subaction_selected;
                share_subaction_selected = false
            }
        }


        //Share
        ToolButton
        {
            id: shareActionButton

            contentItem: Label
            {
                text: "Share"
                color: "black"
            }

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color:
                {
                    if(share_subaction_selected)
                    {
                        return "#b7b7b7"
                    }

                    return shareActionButton.hovered ? "#b7b7b7" : "transparent"
                }
                radius: 2
            }

            onClicked:
            {
                shareClicked();
                share_subaction_selected = !share_subaction_selected;
                edit_subaction_selected = false
            }
        }

        Item
        {
            Layout.fillWidth: true
        }
    }
}
