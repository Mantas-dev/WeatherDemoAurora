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
            leftItem        :null
            background: Rectangle {
                width   : parent.width
                height  : parent.height
                radius  : Theme.dp(30)
                opacity : 0.3
            }

            onTextChanged: {
                listModel.update()
            }
        }

        currentIndex: -1

        model: ListModel {
            id: listModel
            property var continents: ["Africa", "Antarctica", "Asia", "Europe", "North America", "Oceania", "South America"]

            function update() {
                clear()

                if (citiesListView.searchField.text != "" ) {
                    for (var i=0; i<continents.length; i++) {
                        if (continents[i].indexOf(citiesListView.searchField.text) >= 0) {
                            append({"name": continents[i]})
                        }
                    }
                }
            }
        }

        delegate: ListItem {
            Label {
                anchors {
                    left            : parent.left
                    leftMargin      : citiesListView.searchField.textLeftMargin
                    verticalCenter  : parent.verticalCenter
                }
                text: name
            }

            //image://theme/icon-m-favorite
            //image://theme/icon-m-favorite-selected
            IconButton {
                anchors {
                    right           : parent.right
                    rightMargin     : citiesListView.searchField.textLeftMargin
                    verticalCenter  : parent.verticalCenter
                }
                icon.source: "image://theme/icon-m-favorite"
                onClicked: {
                }
            }

            onClicked: {
            }
        }
    }

    Label {
        id              : messageLabel
        anchors.centerIn: parent
        text            : "Введите название города"
        visible         : citiesListView.count == 0
        font.pixelSize  : Theme.fontSizeLarge
    }

    Component.onCompleted: {
        citiesListView.searchField.forceActiveFocus();
    }
}
