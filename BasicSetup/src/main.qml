import QtQuick
import QtQuick.Controls
import Qt.labs.platform
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.settings 1.0
import QtQuick.Shapes 1.5

import com.mycompany.qmlcomponents 1.1

ApplicationWindow
{
    objectName: "mainWindow"

    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")


    header: ToolBar
    {
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
                onTriggered: console.warn("SAVE!")
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
        }
    }

    RowLayout
    {
        anchors.fill: parent
        spacing: 0

        ListView
        {
            Layout.preferredHeight: childrenRect.height
            Layout.preferredWidth: 100

            model : cpp_image_model

            delegate:RowLayout
            {
                Rectangle
                {
                    Layout.preferredHeight: 100
                    Layout.preferredWidth: 100
                    border.color: is_image_selected ? "green": "black"
                    border.width: 3
                    radius: 3

                    Rectangle
                    {
                        anchors.top: parent.top
                        anchors.right: parent.right

                        color:"red"
                        width: 10
                        height: 10

                        visible: has_annotations
                    }

                    Image
                    {
                        width: parent.width
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: image_path
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            image_path = image_path
                            //is_image_selected = !is_image_selected;
                        }
                    }
                }
            }
        }

        Rectangle
        {
            id: main_viewer
            Layout.preferredHeight: 400
            Layout.preferredWidth: 400

            color: "black"
            //currently selected image
            Image
            {
                id: main_image
                anchors.fill: parent

                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignCenter
                source: cpp_datamanager? cpp_datamanager.path : ""
            }

            AnnotationHandler
            {
                objectName: "annotationHandlerQMl"
                anchors.fill: parent
            }
        }

        ListView
        {
            Layout.preferredHeight: childrenRect.height
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft

            model : cpp_dummy_model

            delegate:RowLayout
            {
                Rectangle
                {
                    Layout.preferredHeight: 100
                    Layout.preferredWidth: 100
                    border.color: "black"
                    radius: 3

                    Text
                    {
                        text: annotation_id
                    }
                }
            }
        }

        Rectangle
        {
            width: 20
            height: 20
            MouseArea
            {
                anchors.fill: parent
                propagateComposedEvents: true
            }
        }
    }
}
