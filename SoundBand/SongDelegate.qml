import QtQuick 2.9
import "./abstract" as Abstract

Item {
    width: parent.width
    height: 40
    function init(image, name){
        img.source = image;
        text.text = name;
    }

    function getName(){
        return text.text;
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

        Abstract.BaseText {
            id:text
            text: "name"
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                clicked();
            }
        }
        anchors.fill: parent;
    }
}
