import QtQuick

Rectangle
{
    id: delegate

    width: 245
    height: 200
    border.color:
    {
        if(is_image_selected)
        {
            return "#29e817"
        }
        else
        {
            return delegate_are.containsMouse ? "#8e8e8e"
                                              : "black"
        }

    }

    border.width: 2
    color: "black"

    Image
    {
        id:image

        anchors.centerIn: parent
        width: parent.width - 15
        height: parent.height - 10
        source: "image://cpp_image_provider/" + image_id
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
            source: "qrc:/images/annotation.svg"
            fillMode: Image.PreserveAspectFit
        }

        visible: has_annotations
    }

    MouseArea
    {
        id: delegate_are
        anchors.fill: parent

        hoverEnabled: true

        onClicked:
        {
            image_path = image_path
        }
    }
}
