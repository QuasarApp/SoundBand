import QtQuick 2.4
import QtQuick.Controls 2.0

Item{

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
