import QtQuick 2.4
import QtQuick.Controls 2.0
import "./base" as Base


Item {
    id: playListsControl
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
        var playlists = [];
        playlists = syncEngine.allPlayLists();

        switch_pane(playListsControlSource);

        for(var i = 0; i < playlists.length; i++){
            var temp = Qt.createComponent("PlayListDelegate.qml");
            if(temp.status === Component.Ready){
                var obj = temp.createObject();
                obj.init(playlists[i], i);
                addItem(obj);
            }
        }
    }

    function switch_pane(object){
        for(var i = 0; i < playListsControl.children.length; i++){
            playListsControl.children[i].visible = false;
        }
        object.visible = true;
    }
    Item {
        id: playListsControlSource

        GroupBox {
            id: controlBox
            title: qsTr("Your playlists")
            height: 100;
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            Button{
                id:add
                width: parent.width / 3
                text: qsTr("Add")
                anchors.top:parent.top
                anchors.left: parent.left

                onClicked: {

                }

            }

            Button{
                id:edit
                width: parent.width / 3
                text: qsTr("edit")
                anchors.top:parent.top
                anchors.left: add.right

                onClicked: {
                    editPlayList.visible = true;
                    editPlayList.name = model.get(playLists.selectedItem).text;
                }

            }

            Button{
                id:remove
                width: parent.width / 3
                text: qsTr("remove")
                anchors.top:parent.top
                anchors.left: edit.right

                onClicked: {

                }

            }
        }

        ListView {
            id: playLists

            property int selectedItem: 0
            anchors.top: controlBox.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            delegate: PlayListDelegate{
            }

            model: ListModel {
                id: model;

            }

        }
        anchors.fill: playListsControl;
    }



    PlayListEditPane{

        id: editPlayList;
        visible: false;
        anchors.fill: playListsControl;
    }

    FileDialog {
        id: fileDialog;
        anchors.fill: playListsControl
        showHidden: false;
        visible: false;
        showDotAndDotDot: false
        nameFilters: "*.mp3"
        onFilesSelected: {
            for(var i = 0; i< selectedFiles.length; i++){
                syncEngine.play(selectedFiles[i]);
            }
            switch_pane(editPlayList);
          //  messageDialog.text = "Cannot open file "+ currentFolder() + "/" + fileName
          //  messageDialog.open()
        }
    }

}
