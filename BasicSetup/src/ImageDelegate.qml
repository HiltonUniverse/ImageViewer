import QtQuick

Rectangle
{
    id: delegate

    width: 245
    height: 215
    border.color: is_image_selected ? "#29e817"
                                    : "black"
    border.width: 2
    color: "black"

    Image
    {
        id:image

        anchors.centerIn: parent
        width: 230
        height: 210
        source: image_path
        fillMode: Image.PreserveAspectFit
    }

    Rectangle
    {
        id: annotation_symbol
        anchors.right: parent.right
        anchors.top: parent.top
        height: 10
        width: 10
        color: "red"

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
