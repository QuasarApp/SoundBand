import QtQuick 2.10
import QtQuick.Controls 2.3

Page {
    id: page
    width: 600
    height: 400
    Header{
        id:header
        height: 300
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
}
    CurentPlayList{
        id:curentPlayList
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
    }

}
