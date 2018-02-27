import QtQuick 2.4
import QtQuick.Controls 2.0

Item {
    property color baseColor: "#343434"
    property color color: "#ffffff"
    property bool active: false
    property int lightPower: 15
    property int size: 50
    width: size;
    height: size;
    id: statusIndicator

    Rectangle{
        id: base
        color: (parent.active)? parent.color: baseColor
        border.color: (parent.active)? parent.color: "#b7b7b7"
        border.width: width * 0.1;
        radius: width / 2 ;
        anchors.left: parent.left
        anchors.leftMargin: lightPower
        anchors.right: parent.right
        anchors.rightMargin: lightPower
        anchors.top: parent.top
        anchors.topMargin: lightPower
        anchors.bottom: parent.bottom
        anchors.bottomMargin: lightPower
    }
}

