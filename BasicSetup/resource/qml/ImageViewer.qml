import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.mycompany.qmlcomponents 1.1

Rectangle
{
    id: image_viewer
    color: "#666666"

    signal doubleClicked;
    property alias drawing_canvas: drawing_canvas

    MouseArea
    {
        anchors.fill: parent
        onDoubleClicked: image_viewer.doubleClicked();
    }

    Rectangle
    {
        id: main_viewer
        height: 600
        width: 800

        color: "black"
        anchors.centerIn: parent

        //currently selected image
        Image
        {
            id: main_image
            anchors.fill: parent

            fillMode: Image.PreserveAspectFit
            Layout.alignment: Qt.AlignCenter
            //this ensures that edited image is displayed
            cache: false
        }

        Connections
        {
            //Reference to C++ object that triggers the signal
            target: cpp_image_model
            function onActiveImageChanged()
            {
                main_image.source = ""
                main_image.source = cpp_image_model && (cpp_image_model.active_image_id != "") ? "image://cpp_image_provider/" + cpp_image_model.active_image_id : ""
            }
        }
    }

    AnnotationHandler
    {
        id: drawing_canvas
        objectName: "annotationHandlerQMl"
        anchors.fill: main_viewer
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
                source: splitView.handle === null ? "qrc:/images/collapse.png"
                                                 : "qrc:/images/expand.png";
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
                source: "qrc:/images/reset.svg";
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
                    cpp_reset_annotation_handler.start()
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
}
