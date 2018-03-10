import QtQuick 2.4
import QtQuick.Controls 2.0

Text{
    verticalAlignment: Text.AlignVCenter
    Theme{
        id: theme;
    }
    font.pixelSize: theme.fontSize;
}
