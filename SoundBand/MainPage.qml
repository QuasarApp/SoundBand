import QtQuick 2.9
import QtQuick.Controls 2.2
import SyncEngine 1.0

Page {
    id: mainPage
    width: 600
    height: 400

    property string curentPlayListName: "all"

    function onLoaded(){
        playListsControl.init();
        curentPlayList.init();
    }

    SyncEngine{
        id: syncEngine

    }

    Header{
        id:header
        height: 300
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
    }

    Item{

        SwipeView {
            id: swipeView
            anchors.fill: parent
            currentIndex: 1

            ServerListPage{
                id:serverListPage;
            }

            CurentPlayList {
                id:curentPlayList;

            }

            PlayListsControl{
                id:playListsControl;


            }

        }

        PageIndicator {
            id: indicator

            count: swipeView.count
            currentIndex: swipeView.currentIndex

            anchors.bottom: swipeView.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
    }

}
