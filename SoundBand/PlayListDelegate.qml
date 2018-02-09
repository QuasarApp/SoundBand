import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4

Item {
    x: 5
    width: playLists.width
    height: 20
    property bool isSelected: false
    property string text: "name not changed"

    function init(name){
        text = name;
    }

    StatusIndicator{
        id: indicator
        width: parent.height
        color: "#4fc1e9"
        active: isSelected;
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

    Label {
        id: label
        text: parent.text;
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.left: indicator.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            isSelected = !isSelected;
        }
    }
}
