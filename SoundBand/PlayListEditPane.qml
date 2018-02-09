import QtQuick 2.4
import QtQuick.Controls 2.2

Item {

    id: playListPane;
    property string name: "no selected"
    property var selectedSongs : []

    signal select();

    function addItem(obj){
        model.append(obj);
    }

    function removeItem(obj){
        model.remove(obj);
    }

    function clear(){
        model.clear()
    }

    Text{
        id:name
        height: 30
        text: (parent.name)
        anchors.top: parent.top
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

    Row{
        id: buttons;
        height: 30
        Button{
            id: ok
            text: qsTr("ok")
            onCanceled: {
                for(var i = 0; i < model.count; i++ ){
                    if(model.get(i).isSelected){
                        selectedSongs.push(model.get(i).getName());
                    }
                }
                select();
                playListPane.visible = false;
            }
        }
        Button{
            id: cancel;
            text: qsTr("cancel")
            onCanceled: {
                playListPane.visible = false;
            }
        }
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

}
