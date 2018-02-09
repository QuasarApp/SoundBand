import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {

    visible: true
    width: 340
    height: 480
    title: qsTr("Tabs")

    Loader {
        id: core
        source: "MainPage.qml"
        anchors.fill: parent
        onLoaded: {
            item.onLoaded()
        }
    }



}
