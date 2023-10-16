import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.mycompany.qmlcomponents 1.1

Rectangle
{
    id: image_viewer
    color: "#666666"

    signal doubleClicked;
    property alias can_draw: drawing_canvas.can_draw

    MouseArea
    {
        anchors.fill: parent
        onDoubleClicked: image_viewer.doubleClicked();
    }

    Rectangle
    {
        id: main_viewer
        height: 400
        width: 600

        color: "black"
        anchors.centerIn: parent

        //currently selected image
        Image
        {
            id: main_image
            anchors.fill: parent

            fillMode: Image.PreserveAspectFit
            Layout.alignment: Qt.AlignCenter
            source: cpp_datamanager ? cpp_datamanager.path : ""
        }
    }

    //put below the MouseArea of main view that activates on double click so it's rendered on top
    RowLayout
    {
        anchors.right: parent.right
        anchors.top: parent.top

        Rectangle
        {
            id: size_action_rect

            width: 18
            height: 18

            color: size_action_mouse_are.containsMouse ? "#bfbfbf"
                                                       : "#808080"
            border.color: "#373737"
            radius: 2

            Image
            {
                source: splitView.handle === null ? "file:///Users/Hilton/Desktop/collapse.png"
                                                 : "file:///Users/Hilton/Desktop/expand.png";
                width: parent.width
                height: parent.height
            }

            MouseArea
            {
                id: size_action_mouse_are
                anchors.fill: parent;
                hoverEnabled: true

                onClicked:
                {
                    //change icon from maximize to minimize and vice versa
                    self.updateSplitViewSize();
                }
            }

            ToolTip
            {
                visible: size_action_mouse_are.containsMouse
                text: splitView.handle === null ? qsTr("Minimize")
                                                : qsTr("Maximize")

                background: Rectangle
                {
                    color: Qt.rgba(0.74, 0.74, 0.74, 0.7)
                    anchors.fill: parent
                    radius: 2
                }
            }
        }

        Rectangle
        {
            id: reset_action

            width: 18
            height: 18

            color: reset_action_mouse_area.containsMouse ? "#bfbfbf"
                                                         : "#808080"
            border.color: "#373737"
            radius: 2

            Image
            {
                source: "file:///Users/Hilton/Desktop/reset.svg";
                width: parent.width
                height: parent.height
            }

            MouseArea
            {
                id: reset_action_mouse_area

                anchors.fill: parent;
                hoverEnabled: true

                onClicked:
                {
                    console.warn("RESET ANNOTATIONS!");
                }
            }

            ToolTip
            {
                visible: reset_action_mouse_area.containsMouse
                text: qsTr("Reset")

                background: Rectangle
                {
                    color: Qt.rgba(0.74, 0.74, 0.74, 0.7)
                    anchors.fill: parent
                    radius: 2
                }
            }
        }
    }

    AnnotationHandler
    {
        id: drawing_canvas
        objectName: "annotationHandlerQMl"
        anchors.fill: main_viewer
    }
}
