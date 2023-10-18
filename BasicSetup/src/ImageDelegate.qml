import QtQuick

Rectangle
{
    id: delegate

    width: 245
    height: 200
    border.color: is_image_selected ? "#29e817"
                                    : "black"
    border.width: 2
    color: "black"

    Image
    {
        id:image

        anchors.centerIn: parent
        width: parent.width - 15
        height: parent.height - 10
        source: image_path
        fillMode: Image.PreserveAspectFit
    }

    Rectangle
    {
        id: annotation_symbol
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        height: 15
        width: 15
        color: "transparent"
        Image
        {
            anchors.fill: parent
            source: "file:///Users/Hilton/Desktop/annotation.svg"
            fillMode: Image.PreserveAspectFit
        }

        visible: has_annotations
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            image_path = image_path
        }
    }
}
