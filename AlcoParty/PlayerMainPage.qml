import QtQuick 2.10
import QtQuick.Controls 2.3

Page {
    id: page
    width: 600
    height: 400

    header: Label {
        text: qsTr("AlcoParty")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }
}
