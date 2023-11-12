import QtQuick
import QtQuick.Layouts

Rectangle
{
    id: first_view

    color: "#444"
    z: 1

    ColumnLayout
    {
        spacing: 1

        anchors.fill: parent

        anchors.margins: 12

        RowLayout
        {
            BrowseImage
            {
                onBrowseAccepted: function(selectedFolder)
                {
                    cpp_image_model.loadFromFolder(selectedFolder)
                }
            }

            //Load
            Rectangle
            {
                objectName: "loadFile"

                Layout.preferredWidth: 75
                Layout.preferredHeight: 20
                color: browse_mouse_area.containsMouse ? "#67686a"
                                                       : "#6f7073"
                border.color: "#6f7073"
                radius: 3

                Text
                {
                    anchors.centerIn: parent

                    text: qsTr("Last Saved")
                    color: "#FFFFFF"
                }

                MouseArea
                {
                    id: browse_mouse_area

                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked:
                    {
                        cpp_load_handler.start()
                    }
                }
            }
        }

        Rectangle
        {
            objectName: "gridViewHolder"
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height - 20
            Layout.alignment: Qt.AlignTop

            color:"transparent"
            border.color: "transparent"

            DropArea
            {
                id: drop_area

                anchors.fill: parent

                onDropped: function(drop)
                {
                    if(drop.hasUrls)
                    {
                        cpp_image_model.loadDraggedDroppedImages(drop.urls)
                    }
                }
                enabled: grid_view.count == 0
            }

            Text
            {
                objectName: "noDataText"

                text: qsTr("Browse or \ndrag and drop 2d images")

                anchors.fill: parent

                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: "gray"
                font.bold: true
                font.pixelSize: 13

                visible: grid_view.count == 0
            }

            GridView
            {
                id: grid_view

                model: cpp_image_model

                topMargin: 10
                cellWidth: 250
                cellHeight: 220

                width: parent.width;
                height: parent.height

                contentWidth: width - 2 * parent.border.width;
                contentHeight: height - 2 * parent.border.height;

                clip: true

                delegate: ImageDelegate
                {
                }
            }
        }
    }
}
