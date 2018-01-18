import QtQuick 2.10
import QtQuick.Controls 2.3

ApplicationWindow {
    visible: true
    width: 340
    height: 480
    title: qsTr("Tabs")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: 1

        ServerListPage{
        }

        MainPage {
        }

        PlayListsControl{
        }

    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Servers Page")
        }

        TabButton {
            text: qsTr("Main Page")
        }
        TabButton {
            text: qsTr("Play Lists Control Page")
        }
    }
}
