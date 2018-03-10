
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Extras 1.4
import Qt.labs.folderlistmodel 2.2
import QtQuick.Window 2.0
import "./base" as Base

import "base/utils.js" as Utils

Item {
	id:picker
    signal filesSelected;
	readonly property real textmargin: Utils.dp(Screen.pixelDensity, 8)
	readonly property real textSize: Utils.dp(Screen.pixelDensity, 10)
	readonly property real headerTextSize: Utils.dp(Screen.pixelDensity, 12)
	readonly property real buttonHeight: Utils.dp(Screen.pixelDensity, 24)
	readonly property real rowHeight: Utils.dp(Screen.pixelDensity, 36)
    readonly property real rowWidth: parent.width;
	readonly property real toolbarHeight: Utils.dp(Screen.pixelDensity, 48)
    property var selectedFiles: []
    property bool showDotAndDotDot: false
	property bool showHidden: true
	property bool showDirsFirst: true
	property string folder: "file:///sdcard"
	property string nameFilters: "*.*"

	function currentFolder() {
		return folderListModel.folder;
	}

	function isFolder(fileName) {
		return folderListModel.isFolder(folderListModel.indexOf(folderListModel.folder + "/" + fileName));
	}

	function canMoveUp() {
		return folderListModel.folder.toString() !== "file:///";
	}

    function map(obj){
        var index = selectedFiles.indexOf(obj);
        if(index === -1){
            selectedFiles.push(folderListModel.folder + "/" + obj);
            return true;
        }else{
            selectedFiles.splice(index,1);
            return false
        }
    }

    function isSelected(name){
        return selectedFiles.indexOf(name) !== -1;
    }

	function onItemClick(fileName) {

        if(!isFolder(fileName)){
            return map(fileName);
        }

		if(fileName === ".." && canMoveUp()) {
			folderListModel.folder = folderListModel.parentFolder
		} else if(fileName !== ".") {
			if(folderListModel.folder.toString() === "file:///") {
				folderListModel.folder += fileName
			} else {
				folderListModel.folder += "/" + fileName
			}
		}
        selectedFiles = [];
        return false;
	}

    ListView {

        id: view
        anchors.top: toolbar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        Component {
            id: fileDelegate

            Item {
                height: rowHeight
                width: view.width
                id: item

                Rectangle {
                    color: Qt.rgba(0,0,0,0)
                    id: rectangle;
                    anchors.fill: item

                    MouseArea {
                        anchors.fill: rectangle
                        onClicked: {
                            indicator.active = onItemClick(fileNameText.text)
                        }
                    }

                    Text {
                        id: fileNameText
                        height: width
                        anchors.left: image.right
                        anchors.top: rectangle.top
                        anchors.bottom: rectangle.bottom
                        anchors.right: rectangle.right
                        text: fileName !== undefined ? fileName : ""
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    StatusIndicator{
                        id: indicator
                        width: height * 0.9
                        color: "#4fc1e9"
                        visible: !isFolder(fileName)
                        active: isSelected(fileName);
                        anchors.right: rectangle.right
                        anchors.leftMargin: textmargin
                        anchors.verticalCenter: rectangle.verticalCenter

                    }

                    Image {
                        id: image
                        height: buttonHeight
                        width: height
                        anchors.left: rectangle.left
                        anchors.leftMargin: textmargin
                        anchors.verticalCenter: rectangle.verticalCenter
                        source: isFolder(fileNameText.text) ? "/icons/res/folder.png" : "/icons/res/file.png"
                    }
                }
            }
        }

        model: folderListModel
        delegate: fileDelegate

        ScrollIndicator.horizontal: ScrollIndicator { }
        ScrollIndicator.vertical: ScrollIndicator { }
    }

	Rectangle {
		id: toolbar
		anchors.right: parent.right
		anchors.left: parent.left
		anchors.top: parent.top
		height: toolbarHeight
        color: Utils.backgroundColor()

        Button {
            id: done
            text: qsTr("Done");
            anchors.right: parent.right
            anchors.rightMargin: buttonHeight
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            enabled: canMoveUp()
            onClicked: {
                filesSelected();
            }
        }

		Text {
			id: filePath
            text: folderListModel.folder.toString().replace("file:///", " ► ").replace(new RegExp("/",'g'), " ► ")
			renderType: Text.NativeRendering
			elide: Text.ElideMiddle
            anchors.right: done.left
			font.italic: true
			font.bold: true
			verticalAlignment: Text.AlignVCenter
			anchors.left: parent.left
			anchors.leftMargin: buttonHeight
			anchors.bottom: parent.bottom
			anchors.top: parent.top
			font.pixelSize: textSize

            MouseArea{
                anchors.fill: parent;
                onClicked: {
                    if(canMoveUp) {
                        folderListModel.folder = folderListModel.parentFolder
                    }
                }
            }
		}
	}

	FolderListModel {
		id:  folderListModel
		showDotAndDotDot: picker.showDotAndDotDot
		showHidden: picker.showHidden
		showDirsFirst: picker.showDirsFirst
		folder: picker.folder
		nameFilters: picker.nameFilters
	}

}
