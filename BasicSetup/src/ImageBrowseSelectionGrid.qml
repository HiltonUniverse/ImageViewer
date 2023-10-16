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

        BrowseImage
        {
            onBrowseAccepted: function(selectedFolder)
            {
                cpp_image_model.loadFromFolder(selectedFolder)
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
