import QtQuick 2.7
import QtQuick.Controls 2.0
import "./base" as Base


Item {

    Button{
        id:listen
        width: parent.width
        text: qsTr("Add")
        anchors.top:parent.top
        anchors.left: parent.left

    }

    ListView {
        id: listView

        model: ListModel {

        }

        delegate: ServerDelegate{

        }
        anchors.top:listen.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
