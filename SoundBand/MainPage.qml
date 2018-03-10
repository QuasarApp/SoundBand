import QtQuick 2.7
import QtQuick.Controls 2.0

Page {
    id: mainPage
    width: 600
    height: 400

    property string currentPlayListName: "all"

    function onLoaded(){

    }
    Item{

        SwipeView {
            id: swipeView
            anchors.fill: parent
            currentIndex: 1

            ServerListPage{
                id:serverListPage;
            }

            CurrentPlayList {
                id:currentPlayList;

                onCyrentsongChanged:{
                    header.changeSong(id, name);
                }

            }

            PlayListsControl{
                id:playListsControl;

            }

        }

        PageIndicator {

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

    Header{
        id:header
        height: 300
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
    }



}
