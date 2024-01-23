import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    allowedOrientations : Orientation.All
    backgroundColor     : "#359cbb"

    SilicaListView {
        id : citiesListView
        anchors {
            fill        : parent
            topMargin   : Theme.dp(20)
            leftMargin  : Theme.horizontalPageMargin
            rightMargin : Theme.horizontalPageMargin
        }

        property alias searchField: citiesListView.headerItem

        header: SearchField {
            width           : parent.width
            height          : Theme.dp(70)
            placeholderText : "Поиск"
            placeholderColor: "white"
            color           : "white"
            leftItem        : null
            background: Rectangle {
                width   : parent.width
                height  : parent.height
                radius  : Theme.dp(30)
                opacity : 0.3
            }

            onTextChanged: {
                UILink.searchCitiesModel.refresh(text);
            }
        }

        currentIndex: -1
        model: UILink.searchCitiesModel

        delegate: ListItem {
            Label {
                anchors {
                    left            : parent.left
                    leftMargin      : citiesListView.searchField.textLeftMargin
                    verticalCenter  : parent.verticalCenter
                }
                text    : cityName
                color   : "white"
            }

            IconButton {
                anchors {
                    right           : parent.right
                    rightMargin     : citiesListView.searchField.textLeftMargin
                    verticalCenter  : parent.verticalCenter
                }
                icon.source : isFavorite ? "image://theme/icon-m-favorite-selected"
                                         : "image://theme/icon-m-favorite"
                icon.color  : "white"
                onClicked: {
                    model.isFavorite = !model.isFavorite;
                }
            }

            onClicked: {
                UILink.requestShowWeatherInfo(model.latitude, model.longitude);
                pageStack.push(Qt.resolvedUrl("MainPage.qml"))
            }
        }
    }

    Label {
        id              : messageLabel
        anchors.centerIn: parent
        text            : "Введите название города"
        visible         : citiesListView.count == 0
        font.pixelSize  : Theme.fontSizeLarge
        color           : "white"
    }

    states: [
        State {
            when: citiesListView.searchField.text !== "" && citiesListView.count == 0
            PropertyChanges {
                target  : messageLabel
                text    : "Ничего не найдено"
            }
        }
    ]

    Component.onCompleted: {
        citiesListView.searchField.forceActiveFocus();
    }
}
