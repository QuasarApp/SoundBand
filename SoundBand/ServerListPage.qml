import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0

import "./base" as Base
import "base/utils.js" as Utils


Item {

    readonly property real rowHeight: Utils.dp(Screen.pixelDensity, 36)
    readonly property real rowWidth: parent.width;
    readonly property real textmargin: Utils.dp(Screen.pixelDensity, 8)
    readonly property real textSize: Utils.dp(Screen.pixelDensity, 10)
    readonly property real buttonHeight: Utils.dp(Screen.pixelDensity, 24)

    function onItemClick(index) {
        syncEngine.listen(index);
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

                    Button {
                        text: qsTr("listen")
                        onClicked: {
                            onItemClick(index)
                        }

                        anchors.right: rectangle.right
                        anchors.leftMargin: textmargin
                        anchors.verticalCenter: rectangle.verticalCenter
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
