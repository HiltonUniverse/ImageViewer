import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls

RowLayout
{
    signal browseAccepted(var selectedFolder);

    TextField
    {
        id: text_field

        placeholderText: "Browse images.."

        Keys.onPressed: (event) =>
                        {
                            if(event.key === Qt.Key_Escape)
                            {
                                focus_scope.forceActiveFocus()
                            }
                        }
    }

    FocusScope
    {
        id: focus_scope
    }

    Rectangle
    {
        objectName: "browseItemsHolder"

        Layout.preferredWidth: 40
        Layout.preferredHeight: 20
        color: browse_mouse_area.containsMouse ? "#67686a"
                                               : "#6f7073"
        border.color: "#6f7073"
        radius: 3

        Text
        {
            anchors.centerIn: parent

            text: qsTr("Open")
            color: "#FFFFFF"
        }

        MouseArea
        {
            id: browse_mouse_area

            anchors.fill: parent
            hoverEnabled: true

            onClicked:
            {
                folderDialog.open();
            }
        }

        FolderDialog
        {
            id: folderDialog

            onAccepted:
            {
                text_field.text = currentFolder
                browseAccepted(currentFolder);
                focus_scope.forceActiveFocus();
            }
        }
    }
}
