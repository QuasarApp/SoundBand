import QtQuick 2.7
import QtQuick.Controls 2.0

ApplicationWindow {

    id: applicationWindow
    visible: true
    width: 440
    height: 680
    title: qsTr("Tabs")

    MainPage {
        id: core
        anchors.fill: parent
    }

    Component.onCompleted: {
        core.onLoaded();
    }


}
