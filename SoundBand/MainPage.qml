import QtQuick 2.10
import QtQuick.Controls 2.3
import SyncEngine 1.0

Page {
    id: page
    width: 600
    height: 400

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

                function init(){
                    var listOfSong = [];
                    listOfSong = syncEngine.curentPlayList();

                    for(var i = 0; i < listOfSong.length; i++){
                        var temp = Qt.createComponent("SongDelegate.qml");
                        if(temp.status === Component.Ready){
                            var obj = temp.createObject();
                            var songName = listOfSong[i];
                            obj.init(syncEngine.songImageByName(songName), songName);
                            parent.addItem(obj);
                        }
                    }
                }
            }

            PlayListsControl{
                id:playListsControl;

                function init(){
                    var playlists = [];
                    playlists = syncEngine.allPlayLists();

                    for(var i = 0; i < playlists.length; i++){
                        var temp = Qt.createComponent("PlayListDelegate.qml");
                        if(temp.status === Component.Ready){
                            var obj = temp.createObject();
                            obj.init(playlists[i]);
                            parent.addItem(obj);
                        }
                    }
                }
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
