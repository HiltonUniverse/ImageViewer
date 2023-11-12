import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle
{
    id: shareSubActions
    color: "#828282"
    RowLayout
    {
        anchors.fill: parent
        spacing: 0
        //UNDO
        Action
        {
            id: shareImageAction
            icon.name: "share-image"
            icon.source: "file:///Users/Hilton/Desktop/share_image.svg"
            onTriggered: console.warn("SHARE IMAGE!")
        }

        ToolButton
        {
            id: shareImageToolButton
            action: shareImageAction

            background: Rectangle
            {
                width: parent.width
                height: parent.height
                color: shareImageToolButton.hovered ? "#b7b7b7" : "transparent"
                radius: 2
            }

            ToolTip
            {
                visible: shareImageToolButton.hovered
                text: qsTr("Share Image")

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
