import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0

import "./base" as Base
import "base/utils.js" as Utils

Item {
    id: headerForm

    property int currentSongId: 0
    property string curentSongName: qsTr("Song is not selected")

    function changeSong(id, name){
        currentSongId = id;
        curentSongName = name;
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
        text: curentSongName
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
        height: 100
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
        }

        Button {
            id: play
            width: parent.width/3
            text: qsTr("Play")
            anchors.left: prev.right
        }

        Button {
            id: next
            width: parent.width/3
            text: qsTr(">>")
            anchors.right: parent.right
        }

        Slider {
            id: valume
            value: 0.5
            width: parent.width / 4;
            anchors.bottom: progress.top
            anchors.top: next.bottom
            anchors.left: parent.left
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
            anchors.right: parent.right
        }

        Slider {
            id: progress
            value: 0.5
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
        }
    }



}
