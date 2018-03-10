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


                        MouseArea{
                            onPressAndHold: {
                                popup.y = mouseY
                                popup.open();
                            }
                            anchors.fill: parent;
                        }

                        Popup {
                            id: popup
                            modal: true
                            focus: true
                            x:parent.width / 2 - Utils.dp(Screen.pixelDensity, 75)
                            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                            Rectangle {
                                height: Utils.dp(Screen.pixelDensity, 50)
                                color: Utils.backgroundAltColor()
                                width: Utils.dp(Screen.pixelDensity, 150)
                                radius: Utils.dp(Screen.pixelDensity, 2)

                                Button {
                                    id: editButton
                                    text: qsTr("Edit")
                                    height: parent.height/2
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    onClicked: {
                                        switch_pane(editPlayList);
                                        popup.close()
                                    }
                                }

                                Button {
                                    text: qsTr("Remove")
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.bottom: parent.bottom
                                    height: parent.height/2
                                    onClicked: {
                                        popup.close()

                                    }
                                }
                            }


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

        Rectangle{
            id: controlBox
            height: 40;
            border.width: 1
            color: Utils.backgroundColor()

            Base.BaseText{

                text: qsTr("Your playlists")
                anchors.top:parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 20;
                anchors.right: add.left
            }

            Button{
                id:add
                width: height
                text: qsTr("+")
                anchors.top:parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                onClicked: {

                }

            }
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
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
