import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    id: headerForm
    Image {
        id: viewSong
        fillMode: Image.PreserveAspectFit
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: groupBox.top
        anchors.left: parent.right
        source: "image/res/logo.png"
    }
    Text {
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
        height: 200
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        title: qsTr("AlcoParty")

        Button {
            id: prev
            Image {
                id: _Iprev
                source: "file"
                anchors.fill: parent
            }
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
            Image {
                id: _Inext
                source: "file"
            }
            width: parent.width/4
            text: qsTr(">>")
            anchors.right: parent.right
        }

        Text {
            id: description
            color: "#b5abab"
            text: qsTr("Song is not selected")
            styleColor: "#a29a9a"
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.bottom: progress.top
            anchors.top: next.bottom
            anchors.left: parent.left
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
