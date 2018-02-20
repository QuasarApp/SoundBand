import QtQuick 2.9
import QtQuick.Controls 2.2
import "./abstract" as Abstract

Item {
    id: headerForm
    Image {
        id: viewSong
        fillMode: Image.PreserveAspectFit
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: groupBox.top
        anchors.left: parent.right
        source: "/image/res/logo.png"
        visible: true;
    }
    Abstract.BaseText {
        id: songName
        color: "#5e5858"
        height: 20
        text: qsTr("Song is not selected")
        styleColor: "#554f4f"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right

        font.pixelSize:16
    }

    GroupBox {
        id: groupBox
        y: 149
        height: 160
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        title: qsTr("SoundBand")

        Button {
            id: prev

            width: parent.width/4
            text: qsTr("<<")
            anchors.left: parent.left
        }

        Button {
            id: play
            width: parent.width/4
            text: qsTr("Play")
            anchors.left: prev.right
        }

        Button {
            id: stop
            width: parent.width/4
            text: qsTr("Stop")
            anchors.right: next.left
        }

        Button {
            id: next
            width: parent.width/4
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

        Abstract.BaseText {
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
