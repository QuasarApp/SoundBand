import QtQuick 2.4
import QtQuick.Controls 2.2

Item{
    function addItem(obj){
        model.append(obj);
    }

    function removeItem(obj){
        model.remove(obj);
    }

    function clear(){
        model.clear()
    }

    function init(){
        var listOfSong = [];
        listOfSong = syncEngine.curentPlayList(mainPage.curentPlayListName);

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

    ListView {
        id: listView
        width: parent.width
        height: parent.height

        model: ListModel {
            id: model;

        }

        delegate: SongDelegate{

        }
        anchors.fill: parent
    }
}
