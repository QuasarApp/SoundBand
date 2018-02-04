import QtQuick 2.4
import QtQuick.Controls 2.3

Item {

    property string name: "no selected"

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
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
