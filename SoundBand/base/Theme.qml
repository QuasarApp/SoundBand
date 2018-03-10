import QtQuick 2.4
import QtQuick.Window 2.2

Item{
    id:theme;
    property int size: 25
    property int fontSizeMax: Screen.desktopAvailableHeight / 50;
    property int fCurrentSize: (applicationWindow.height / size > applicationWindow.width / size)? applicationWindow.width / size : applicationWindow.height / size
    property int fontSize: (fCurrentSize < fontSizeMax)? fCurrentSize: fontSizeMax
}
