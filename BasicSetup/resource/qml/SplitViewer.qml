import QtQuick
import QtQuick.Controls
import Qt.labs.platform
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.settings 1.0

Rectangle
{
    id: split_viewer
    property int parent_width : 0
    property int parent_height : 0

    property alias drawing_canvas: image_viewer.drawing_canvas

    QtObject
    {
        id: self

        function updateSplitViewSize()
        {
            if(splitView.handle)
            {
                image_selection_grid.visible = false
                splitView.handle = null
                image_viewer.width = parent_width
                image_viewer.height = parent_height
            }
            else
            {
                splitView.handle = handleDelegate
                image_selection_grid.visible = true
            }
        }
    }

    //serialize the place of the handler
    Settings
    {
        id: settings
        property var splitView
    }

    Component.onCompleted: splitView.restoreState(settings.splitView)
    Component.onDestruction: settings.splitView = splitView.saveState()

    Component
    {
        id: handleDelegate
        Rectangle
        {
            implicitWidth: 4
            implicitHeight: 4
            color: SplitHandle.pressed ? "#bfbfbf"
                                       : SplitHandle.hovered ? "#bfbfbf" : "#808080"
        }
    }

    SplitView
    {
        id: splitView

        width: parent.width
        height: parent.height
        clip: true
        handle: handleDelegate

        ImageBrowseSelectionGrid
        {
            id: image_selection_grid

            implicitWidth: 150
            SplitView.minimumWidth: 270
            SplitView.maximumWidth: parent.width - 80
        }

        ImageViewer
        {
            id:image_viewer

            implicitWidth: 50
            onDoubleClicked: self.updateSplitViewSize();
        }
    }
}
