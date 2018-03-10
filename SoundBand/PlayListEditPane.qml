import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Extras 1.4
import QtQuick.Window 2.0

import "./base" as Base
import "base/utils.js" as Utils

Rectangle {

    id: playListPane;
    property string name: "no selected"
    readonly property real rowHeight: Utils.dp(Screen.pixelDensity, 36)
    readonly property real rowWidth: parent.width;
    readonly property real textmargin: Utils.dp(Screen.pixelDensity, 8)
    readonly property real textSize: Utils.dp(Screen.pixelDensity, 10)
    readonly property real buttonHeight: Utils.dp(Screen.pixelDensity, 24)

    function showPlayList(name){
        playListModel.setNewList(name);
        this.name = name;
    }

    color: Utils.backgroundColor()

    ListView {
        id: listView

        model: playListModel

        Component {
            id: playListDelegate

            Item {
                height: rowHeight
                width: listView.width
                id: item

                Rectangle {
                    color: Utils.baseColor();
                    id: rectangle;
                    anchors.fill: item

                    MouseArea {
                        anchors.fill: parent;

                        onClicked: {
                            indicator.active = playListModel.select(songId)
                        }
                    }

                    Text {
                        id: textNameSong
                        height: width
                        anchors.left: image.right
                        anchors.top: rectangle.top
                        anchors.bottom: rectangle.bottom
                        anchors.right: indicator.left
                        text: songName !== undefined ? songName : ""
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    StatusIndicator{
                        id: indicator
                        width: height * 0.9
                        color: "#4fc1e9"
                        active: playListModel.isSelected(songId);
                        anchors.right: rectangle.right
                        anchors.leftMargin: textmargin
                        anchors.verticalCenter: rectangle.verticalCenter

                    }

                    Image {
                        id: image
                        height: buttonHeight
                        width: height
                        anchors.left: rectangle.left
                        anchors.leftMargin: textmargin
                        anchors.verticalCenter: rectangle.verticalCenter
                        source: "image://collection/" + songId
                    }
                }
            }
        }

        delegate: playListDelegate

        ScrollIndicator.horizontal: ScrollIndicator { }
        ScrollIndicator.vertical: ScrollIndicator { }

        anchors.top:controlBox.bottom
        anchors.bottom: parent.bottom

        anchors.left: parent.left
        anchors.right: parent.right
    }

    Rectangle{
        id:controlBox
        color: Utils.baseColor()

        height: Utils.dp(Screen.pixelDensity, 40)

        Base.BaseText{
            id:namePalyList
            text: playListPane.name
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: buttons.left
        }

        Rectangle{

            id: buttons;
            height: namePalyList.height
            color: Utils.baseColor()
            width: Utils.dp(Screen.pixelDensity, 160)

            Base.BaseButton{
                id: ok
                text: qsTr("ok")

                onClicked: {

                    switch_pane(playListsControlSource);

                }
                anchors.left: add.right
                anchors.right: parent.right



            }

            Base.BaseButton{
                id: add;
                text: qsTr("add")
                onClicked: {
                    switch_pane(fileDialog);
                }
                anchors.left: parent.left
                anchors.rightMargin: 10
            }

            anchors.right: parent.right
        }

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

    }




}
