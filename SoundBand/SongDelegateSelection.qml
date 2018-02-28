import QtQuick 2.7
import QtQuick.Controls 2.0
import "./base" as Base

SongDelegate {

    property bool isSelected: false

//    Base.StatusIndicator{
//        height: parent.height
//        width: parent.height
//        anchors.right: parent.left
//        anchors.verticalCenter: parent
//        color: "#4fc1e9"
//        active: parent.isSelected
//    }

    onClicked: {
        isSelected = !isSelected;
    }
}
