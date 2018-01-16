import QtQuick 2.4
import QtQuick.Controls 2.3

ListView {
    id: listView
    width: parent.width
    height: parent.height
    model: ListModel {

    }

    delegate: SongDelegate{

    }

}
