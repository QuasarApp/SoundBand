import QtQuick 2.4
import QtQuick.Controls 2.0

//    Rectangle{
//        property color baseColor: "#343434"
//        property color activeColor: "#ffffff"
//        property bool active: false
//        property int lightPower: 5
//        property int size: 50

//        id: base
//        color: (active)? activeColor: baseColor
//        border.color: (active)? activeColor: "#b7b7b7"
//        border.width: width * 0.1;
//        radius: width / 2 ;

//    }


Item {
    property color baseColor: "#343434"
    property color color: "#ffffff"
    property bool active: false
    property int lightPower: 15
    property int size: 50
    width: size;
    height: size;
    id: statusIndicator

//    Rectangle{
//        id: light
//        visible: active;
//        RadialGradient {
//            anchors.fill: parent
//            gradient: Gradient {
//                GradientStop {
//                  position: 0.0
//                  color: "#00000000"
//                }
//                GradientStop {
//                  position: 1.0
//                  color: "#FF000000"
//                }
//            }
//        }
//        anchors.fill: parent;
//    }

    Rectangle{
        id: base
        color: (parent.active)? parent.color: baseColor
        border.color: (parent.active)? parent.color: "#b7b7b7"
        border.width: width * 0.1;
        radius: width / 2 ;
        anchors.left: parent.left
        anchors.leftMargin: lightPower
        anchors.right: parent.right
        anchors.rightMargin: lightPower
        anchors.top: parent.top
        anchors.topMargin: lightPower
        anchors.bottom: parent.bottom
        anchors.bottomMargin: lightPower
    }

//    Rectangle{
//        id:mask
//        visible: active
//        RadialGradient {
//            anchors.fill: parent
//            gradient: Gradient {
//                GradientStop { position: 0.0; color: Qt.rgba(1,1,1,0) }
//                GradientStop { position: 0.5; color: Qt.rgba(1,1,1, 0.7)}
//                GradientStop { position: 1; color: Qt.rgba(1,1,1,0)}
//            }
//        }
//        anchors.fill: base;
//    }
}

