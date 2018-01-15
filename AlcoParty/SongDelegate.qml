import QtQuick 2.0

Item {
    x: 5
    width: parent.width
    height: 40
    Row {
        id: row1
        spacing: 10
        Image {
            width: 40
            height: 40
        }

        Text {
            text: name
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
        }
    }
}
