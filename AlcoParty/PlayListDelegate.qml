import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    x: 5
    width: 80
    height: 40
    property bool isSelected: false
    Row {
        id: row1
        spacing: 10
        Rectangle {
            color: (isSelected)?"#00b3f9":"#ffffff"
            radius: 20
            anchors.fill: parent
        }

        Text {
            text: name
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
        }
    }
}
