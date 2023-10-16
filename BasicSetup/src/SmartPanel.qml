import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout
{
    id: root

    spacing: 0

    property alias model: list_view.model;

    QtObject
    {
        id: self

        function getColor()
        {
            return show_hide_smartpanel_area.containsMouse ? "#bfbfbf"
                                                           : "#808080"
        }
    }

    Rectangle
    {
        objectName: "ActionHolderSmartPanel"

        color: "#808080"

        Layout.fillHeight: true
        Layout.minimumWidth: 10

        Rectangle
        {
            objectName: "ShowHideActionSmartPanel"
            width: parent.width
            height: 30
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            color: self.getColor()

            Image
            {
                source: smart_panel.visible ? "file:///Users/Hilton/Desktop/arrow_right.png"
                                            : "file:///Users/Hilton/Desktop/arrow_left.png"

                width: parent.width
                height: parent.height

                fillMode: Image.PreserveAspectFit
            }

            MouseArea
            {
                id: show_hide_smartpanel_area
                objectName: "showHideSmartPanelArea"

                anchors.fill: parent

                hoverEnabled: true

                onExited:
                {
                    hoverEnabled = true
                }

                onClicked:
                {
                    hoverEnabled = false
                    smart_panel.visible = !smart_panel.visible;
                }
            }
        }
    }

    Rectangle
    {
        id: smart_panel

        color: "#444"
        Layout.fillHeight: true
        Layout.minimumWidth: 200

        Text
        {
            text: qsTr("No data to show")

            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            color: "gray"
            font.bold: true
            font.pixelSize: 13
        }

        Flickable
        {
            clip: true

            anchors.fill: parent

            contentHeight: content_layout.implicitHeight

            boundsBehavior: Flickable.StopAtBounds

            ScrollBar.vertical: ScrollBar{}

            ColumnLayout
            {
                id: content_layout

                implicitHeight: title.implicitHeight + list_view.implicitHeight

                spacing: 0

                Rectangle
                {
                    id: title
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 30
                    Layout.topMargin: 5
                    Layout.leftMargin: 23
                    Layout.alignment: Qt.AlignHCenter
                    color: "#444"

                    Text
                    {
                        text: qsTr("FREE HAND ANNOTATIONS")
                        anchors.fill: parent

                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: "white"
                        font.pixelSize: 13
                        font.bold: true
                    }
                }

                ListView
                {
                    id: list_view

                    implicitHeight: childrenRect.height

                    delegate: RowLayout
                    {
                        width: parent ? parent.width
                                      : 0

                        Rectangle
                        {
                            id: delegate_rect

                            Layout.preferredWidth: 170
                            Layout.preferredHeight: 30
                            Layout.margins: 12
                            Layout.alignment: Qt.AlignHCenter
                            color: "#666666"
                            border.color:
                            {
                                if(annotation_selected)
                                {
                                    return "#29e817"
                                }
                                else
                                {
                                    delegate_area.containsMouse ? "#8c8c8c" : "#666666"
                                }
                            }
                            border.width: 2
                            radius: 3

                            Text
                            {
                                color: "white"

                                anchors.fill: parent

                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                text: annotation_id + ": " + annotation_notification
                                wrapMode: Text.Wrap
                            }

                            MouseArea
                            {
                                id: delegate_area

                                anchors.fill: parent
                                hoverEnabled: true

                                onClicked: annotation_selected = !annotation_selected
                            }
                        }
                    }
                }
            }
        }
    }
}
