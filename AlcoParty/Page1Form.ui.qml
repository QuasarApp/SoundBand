import QtQuick 2.10
import QtQuick.Controls 2.3

Page {
    id: page
    width: 600
    height: 400

    header: Label {
        text: qsTr("Servers")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Text {
        id: text1
        y: 174
        text: qsTr("This page is not suported")
        anchors.left: parent.left
        anchors.leftMargin: 0
        font.pixelSize: 12
    }
}
