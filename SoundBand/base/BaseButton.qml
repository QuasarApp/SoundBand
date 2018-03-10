import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import "utils.js" as Utils

Button{
    id: ok
    text: qsTr("ok")
    width: Utils.dp(Screen.pixelDensity, 80)
    height: Utils.dp(Screen.pixelDensity, 40)

}
