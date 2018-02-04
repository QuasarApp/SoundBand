import QtQuick 2.10

Item {
    width: parent.width
    height: 40
    function init(image, name){
        img.source = image;
        text.text = name;
    }

    signal clicked();
    Row {
        id: row1
        spacing: 10
        Image {
            id:img;
            width: 40
            height: 40
        }

        Text {
            id:text
            text: name
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                clicked();
            }
        }
    }
}
