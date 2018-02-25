import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Extras 1.4
import "./base" as Base


Item {
    id:playListDelegate;
    x: 5
    width: playLists.width
    height: 20
    property bool isSelected: playLists.selectedItem == number
    property int number: 0
    property string text: "name not changed"

    function init(name , n){
        text = name;
        number = n;
    }

    Base.StatusIndicator{
        id: indicator
        width: parent.height
        color: "#4fc1e9"
        active: playListDelegate.isSelected;
        anchors.left: playListDelegate.left
        anchors.top: playListDelegate.top
        anchors.bottom: playListDelegate.bottom
    }

    Base.BaseText {
        id: label
        text: playListDelegate.text;
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.left: indicator.right
        anchors.right: playListDelegate.right
        anchors.top: playListDelegate.top
        anchors.bottom: playListDelegate.bottom
    }

    MouseArea{
        anchors.fill: playListDelegate
        onClicked: {
            clicked;
        }
    }
}
