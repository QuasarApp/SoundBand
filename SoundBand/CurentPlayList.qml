import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Window 2.0

import "./base" as Base
import "base/utils.js" as Utils


Item{

    readonly property real rowHeight: Utils.dp(Screen.pixelDensity, 36)
    readonly property real rowWidth: parent.width;
    readonly property real textmargin: Utils.dp(Screen.pixelDensity, 8)
    readonly property real textSize: Utils.dp(Screen.pixelDensity, 10)
    readonly property real buttonHeight: Utils.dp(Screen.pixelDensity, 24)

    property int curentSongId: -1

    signal cyrentsongChanged(int id, string name)

    function onItemClick(id, name) {
        cyrentsongChanged(id, name);
        syncEngine.play(id);
    }

    ListView {
        id: listView
        width: parent.width
        height: parent.height

        model: currentPlayListModel;

        Component {
            id: playListDelegate

            Item {
                height: rowHeight
                width: listView.width
                id: item

                Rectangle {
                    color: (curentSongId === songId)? "#ffffff":Qt.rgba(0,0,0,0)
                    id: rectangle;
                    anchors.fill: item

                    MouseArea{
                        anchors.fill: parent;
                        onClicked: {
                            onItemClick(songId, songName);
                            curentSongId = songId;
                        }
                    }

                    Text {
                        id: playListDelegateNameText
                        height: width
                        anchors.left: image.right
                        anchors.top: rectangle.top
                        anchors.bottom: rectangle.bottom
                        anchors.right: rectangle.right
                        text: songName !== undefined ? songName : ""
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
                        source: "image://collection/" + songId
                    }
                }
            }
        }

        delegate: playListDelegate

        ScrollIndicator.horizontal: ScrollIndicator { }
        ScrollIndicator.vertical: ScrollIndicator { }

        anchors.fill: parent
    }
}
