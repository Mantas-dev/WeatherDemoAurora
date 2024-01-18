import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    allowedOrientations : Orientation.All
    backgroundColor     : "#359cbb"

    Label {
        id : titleLabel
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            topMargin   : Theme.dp(30)
            leftMargin  : Theme.horizontalPageMargin
            rightMargin : Theme.horizontalPageMargin
        }
        text            : "Избранные города"
        font.pixelSize  : Theme.fontSizeExtraLarge
    }

    SilicaListView {
        id : citiesListView
        anchors {
            top         : titleLabel.bottom
            left        : parent.left
            right       : parent.right
            bottom      : parent.bottom
            topMargin   : Theme.dp(16)
            leftMargin  : Theme.horizontalPageMargin
            rightMargin : Theme.horizontalPageMargin
        }
        clip: true
        model: ["Africa", "Antarctica", "Asia", "Europe", "North America", "Oceania", "South America"]

        delegate: ListItem {
            Label {
                anchors {
                    left            : parent.left
                    leftMargin      : citiesListView.searchField.textLeftMargin
                    verticalCenter  : parent.verticalCenter
                }
                text: modelData
            }

            IconButton {
                anchors {
                    right           : parent.right
                    rightMargin     : citiesListView.searchField.textLeftMargin
                    verticalCenter  : parent.verticalCenter
                }
                icon.source: "image://theme/icon-m-delete"
                onClicked: {
                }
            }

            onClicked: {
            }
        }
    }
}
