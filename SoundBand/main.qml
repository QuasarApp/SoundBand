import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {

    id: applicationWindow
    visible: true
    width: 340
    height: 480
    title: qsTr("Tabs")

    MainPage {
        id: core
        anchors.fill: parent
    }

    Component.onCompleted: {
        core.onLoaded();
    }


}
