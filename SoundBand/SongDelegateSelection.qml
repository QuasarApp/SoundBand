import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Extras 1.4

SongDelegate {

    property bool isSelected: false

    Row{
        spacing: 5

    }

    StatusIndicator{
        height: parent.height
        width: parent.height
        anchors.right: parent.left
        anchors.verticalCenter: parent
        color: "#4fc1e9"
        active: parent.isSelected
    }

    onClicked: {
        isSelected = !isSelected;
    }
}
