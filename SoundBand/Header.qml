import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0

import "./base" as Base
import "base/utils.js" as Utils

Item {
    id: headerForm

    property int currentSongId: 0
    property string currentSongName: qsTr("Song is not selected")
    property bool playState: false


    function changeSong(id, name){
        currentSongId = id;
        currentSongName = name;
    }

    Image {
        id: viewSong
        anchors.leftMargin: 0
        fillMode: Image.PreserveAspectCrop
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: songName.top
        anchors.left: parent.left
        source: "image://collection/" + currentSongId;
        visible: true;
    }

    Base.BaseText {
        id: songName
        color: "#5e5858"
        height: 20
        text: currentSongName
        styleColor: "#554f4f"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: groupBox.top
        anchors.bottomMargin: 0
        font.pixelSize:16
    }

    GroupBox {

        id: groupBox
        height: 120
        padding: 0;
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        Rectangle{
            anchors.fill: parent;
            color: Utils.backgroundColor()
        }

        Button {
            id: prev

            width: parent.width/3
            text: qsTr("<<")
            anchors.left: parent.left

            onClicked: {
                syncEngine.prev();
            }
        }

        Button {
            id: play
            width: parent.width/3
            text: (playState)? qsTr("Pause"): qsTr("Play")
            anchors.left: prev.right

            onClicked: {
                syncEngine.pause(playState);
                playState = !playState;
            }

        }

        Button {
            id: next
            width: parent.width/3
            text: qsTr(">>")
            anchors.right: parent.right

            onClicked: {
                syncEngine.next();
            }
        }

        Slider {
            id: valume
            from: 0
            to: 100
            value: syncEngine.getValume()
            width: parent.width / 4;
            anchors.bottom: progress.top
            anchors.top: next.bottom
            anchors.left: parent.left
            onMoved: {
                syncEngine.setValume(value);
            }
        }
        Base.BaseButton{
            property int state: syncEngine.repeat + "";

            id:repeatControl
            text: state + "";
            width: height;
            anchors.verticalCenter: valume.verticalCenter
            anchors.right: parent.right;
            anchors.rightMargin: Utils.dp(Screen.pixelDensity, 5)
            onClicked: {
                state = ++state % 4;
                syncEngine.setRepeat(state)
            }
        }

        Base.BaseText {
            id: valumeText
            color: "#b5abab"
            text: qsTr("Valume")
            styleColor: "#a29a9a"
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: progress.top
            anchors.top: next.bottom
            anchors.left: valume.right
        }

        Slider {
            id: progress
            value: syncEngine.pos
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            onMoved:{
                syncEngine.setPos(value);
            }
        }
    }



}
