import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Extras 1.4

Item {
    x: 5
    width: 80
    height: 40
    property bool isSelected: false
    Row {
        id: row1
        spacing: 10
        StatusIndicator{
            anchors.centerIn: parent
            color: "#4fc1e9"
            active: parent.isSelected
        }

        Text {
            text: name
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
        }
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            isSelected = !isSelected;
        }
    }
}
