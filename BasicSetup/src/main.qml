import QtQuick
import QtQuick.Controls
import Qt.labs.platform
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.settings 1.0
import QtQuick.Shapes 1.5


ApplicationWindow
{
    id: win
    objectName: "mainWindow"

    minimumWidth: 800
    minimumHeight: 650

    visible: true
    title: qsTr("Image viewer")

    //initial size of our main window
    width: 1000
    height: 650

    header: HeaderActions
    {
        onEditClicked:
        {
            shareSubActions.visible = false
            editSubActions.visible = !editSubActions.visible
        }

        onShareClicked:
        {
            editSubActions.visible = false
            shareSubActions.visible = !shareSubActions.visible
        }
    }

    ColumnLayout
    {
        anchors.fill: parent

        spacing: 0

        EditSubActions
        {
            id: editSubActions

            visible: false

            Layout.fillWidth: true
            Layout.preferredHeight: visible ? 30 : 0
            onFreeHandStarted: split_viewer.can_draw = true
            onColorPenSelected: function (color)
            {
                console.warn("COLOR is: ", color)
            }

            onColorBackgroundSelected: function(color)
            {
                console.warn("BACKGROUND COLOR is: ", color)
            }
        }

        ShareImageSubActions
        {
            id: shareSubActions

            visible: false

            Layout.fillWidth: true
            Layout.preferredHeight: visible ? 30 : 0
        }

        RowLayout
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            SplitViewer
            {
                id: split_viewer

                parent_width: win.width
                parent_height: win.height

                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            SmartPanel
            {
                id:smart_panel

                Layout.fillHeight: true
                Layout.minimumWidth: 10
                model: cpp_dummy_model
            }
        }
    }
}
