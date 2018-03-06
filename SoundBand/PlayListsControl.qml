import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Window 2.0

import "./base" as Base
import "base/utils.js" as Utils


Item {
    id: playListsControl

    readonly property real rowHeight: Utils.dp(Screen.pixelDensity, 36)
    readonly property real rowWidth: parent.width;
    readonly property real textmargin: Utils.dp(Screen.pixelDensity, 8)
    readonly property real textSize: Utils.dp(Screen.pixelDensity, 10)
    readonly property real buttonHeight: Utils.dp(Screen.pixelDensity, 24)

    function switch_pane(object){
        for(var i = 0; i < playListsControl.children.length; i++){
            playListsControl.children[i].visible = false;
        }
        object.visible = true;
    }
    Item {
        id: playListsControlSource

        GroupBox {
            id: controlBox
            title: qsTr("Your playlists")
            height: 100;
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            Button{
                id:add
                width: parent.width / 3
                text: qsTr("Create")
                anchors.top:parent.top
                anchors.left: parent.left

                onClicked: {

                }

            }

//            Rectangle{
//                color: "#ffffff";
//                width: parent.width / 3
//                TextEdit{

//                    id:edit
//                    text: qsTr("Create")
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter

//                    anchors.fill: parent;

//                }
//                anchors.top:parent.top
//                anchors.left: add.right
//                radius: width / 4;
//                border.color: "black"
//            }

        }

        ListView {
            id: playLists

            property int selectedItem: 0
            anchors.top: controlBox.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Component {
                id: playListsDelegate

                Item {
                    height: rowHeight
                    width: playLists.width
                    id: item

                    Rectangle {
                        color: Qt.rgba(0,0,0,0)
                        id: rectangle;
                        anchors.fill: item

                        Button {
                            id: editButton
                            text: qsTr("Edit")
                            onClicked: {
                                switch_pane(editPlayList);
                            }

                            anchors.right: rectangle.right
                            anchors.leftMargin: textmargin
                            anchors.verticalCenter: rectangle.verticalCenter
                        }

                        Button {
                            text: qsTr("Remove")
                            onClicked: {

                            }

                            anchors.right: editButton.left
                            anchors.verticalCenter: rectangle.verticalCenter
                        }

                        Text {
                            id: serverNameText
                            height: width
                            anchors.left: image.right
                            anchors.top: rectangle.top
                            anchors.bottom: rectangle.bottom
                            anchors.right: rectangle.right
                            text: playListName !== undefined ? playListName : ""
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        Image {
                            id: image
                            height: buttonHeight
                            width: height
                            anchors.left: rectangle.left
                            anchors.leftMargin: textmargin
                            anchors.verticalCenter: rectangle.verticalCenter
                            source: "/icons/res/folder.png"
                        }
                    }
                }
            }

            delegate: playListsDelegate;

            model: playListsModel;

        }
        anchors.fill: playListsControl;
    }



    PlayListEditPane{

        id: editPlayList;
        visible: false;
        anchors.fill: playListsControl;
    }

    FileDialog {
        id: fileDialog;
        anchors.fill: playListsControl
        showHidden: false;
        visible: false;
        showDotAndDotDot: false
        nameFilters: "*.mp3"
        onFilesSelected: {
            switch_pane(editPlayList);
          //  messageDialog.text = "Cannot open file "+ currentFolder() + "/" + fileName
          //  messageDialog.open()
        }
    }

}
