import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0

import "./base" as Base
import "base/utils.js" as Utils


Item {

    readonly property real rowHeight: Utils.dp(Screen.pixelDensity, 36)
    readonly property real rowWidth: parent.width;
    function onItemClick() {
    }

    Button{
        id:listen
        width: parent.width
        text: qsTr("Refresh")
        anchors.top:parent.top
        anchors.left: parent.left

        onClicked: {
            syncEngine.scan();
        }

    }

    ListView {
        id: listView

        model: serverListModel;

        Component {
            id: serverDelegate

            Item {
                height: rowHeight
                width: listView.width
                id: item

                Rectangle {
                    color: Qt.rgba(0,0,0,0)
                    id: rectangle;
                    anchors.fill: item

                    MouseArea {
                        anchors.fill: rectangle
                        onClicked: {
                            indicator.active = onItemClick()
                        }
                    }

                    Text {
                        id: serverNameText
                        height: width
                        anchors.left: image.right
                        anchors.top: rectangle.top
                        anchors.bottom: rectangle.bottom
                        anchors.right: rectangle.right
                        text: serverName !== undefined ? serverName : ""
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
        delegate: serverDelegate

        ScrollIndicator.horizontal: ScrollIndicator { }
        ScrollIndicator.vertical: ScrollIndicator { }

        anchors.top:listen.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
