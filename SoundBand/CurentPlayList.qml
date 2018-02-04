import QtQuick 2.4
import QtQuick.Controls 2.3

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
