import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4

SongDelegate {

    property bool isSelected: false

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
