import QtQuick 2.4
import QtQuick.Controls 2.2
import FileDialog 1.0

Rectangle {

    id: playListPane;
    property string name: "no selected"
    property var selectedSongs : []

    signal select();
    color: "#5e5c5c"

    function addItem(obj){
        model.append(obj);
    }

    function removeItem(obj){
        model.remove(obj);
    }

    function clear(){
        model.clear()
    }

    function view(curentPlayListName){
        name = curentPlayListName;
        playListPane.visible = true;
        var added = [], all = [];
        added = syncEngine.getPlayList(name);
        all = syncEngine.getPlayList("");
        for(var i = 0; i < all.lenght; i++){

            var temp = Qt.createComponent("SongDelegateSelection.qml");
            if(temp.status === Component.Ready){
                var obj = temp.createObject();
                obj.init("/image/res/logo.png", all[i]);

                if(added.indexOf(all[i]) >= 0){
                    obj.isSelected = true;
                }

                addItem(obj);
            }


        }

    }

    Text{
        id:namePalyList
        height: 30
        text: (parent.name)
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Item{

        FileDialog{
            id: fileDialog

            onFilesUrlsDone:{

            }

            onFileUrlDone:{

            }
        }

        id: buttons;
        height: 30
        Button{
            id: ok
            text: qsTr("ok")
            onClicked: {
                for(var i = 0; i < model.count; i++ ){
                    if(model.get(i).isSelected){
                        selectedSongs.push(model.get(i).getName());
                    }
                }
                select();
                editPlayList.visible = false;
            }
            anchors.left: parent.left
            anchors.leftMargin: 10

        }

        Button{
            id: cancel;
            text: qsTr("cancel")
            onClicked: {
                editPlayList.visible = false;
            }
            anchors.left: ok.right
            anchors.leftMargin: 10;
        }

        Button{
            id: add;
            text: qsTr("add")
            onClicked: {
                fileDialog.open();
                editPlayList.visible = false;
            }
            anchors.right: remove.left
            anchors.rightMargin: 10
        }

        Button{
            id: remove;
            text: qsTr("remove")
            onClicked: {
                editPlayList.visible = false;
            }
            anchors.right: parent.right
            anchors.rightMargin: 10
        }

        anchors.bottom: listView.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    ListView {
        id: listView

        model: ListModel {
            id: model

        }

        delegate: SongDelegateSelection{

        }
        anchors.top:name.bottom
        anchors.bottom: buttons.bottom

        anchors.left: parent.left
        anchors.right: parent.right
    }



}
