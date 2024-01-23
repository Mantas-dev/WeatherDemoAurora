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
        color           : "white"
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
        clip    : true
        model   : UILink.favoritesCitiesModel

        delegate: ListItem {
            Label {
                anchors {
                    left            : parent.left
                    verticalCenter  : parent.verticalCenter
                }
                text    : cityName
                color   : "white"
            }

            IconButton {
                anchors {
                    right           : parent.right
                    verticalCenter  : parent.verticalCenter
                }
                icon.source : "image://theme/icon-m-delete"
                icon.color  : "white"
                onClicked: {
                    UILink.favoritesCitiesModel.remove(index);
                }
            }

            onClicked: {
                UILink.requestShowWeatherInfo(model.latitude, model.longitude);
                pageStack.push(Qt.resolvedUrl("MainPage.qml"))
            }
        }
    }
}
