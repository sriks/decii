import QtQuick 1.0
import Qt.labs.components.native 1.0

ToolButton {
    id: defaultToolButton;
    property string toolId;
    property Item actionHandler;
    flat: true
    onClicked: actionHandler.handleAction(toolId);
}
