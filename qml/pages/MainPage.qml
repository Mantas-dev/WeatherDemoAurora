/*******************************************************************************
**
** Copyright (C) 2022 ru.weather
**
** This file is part of the Weather Demo project.
**
** Redistribution and use in source and binary forms,
** with or without modification, are permitted provided
** that the following conditions are met:
**
** * Redistributions of source code must retain the above copyright notice,
**   this list of conditions and the following disclaimer.
** * Redistributions in binary form must reproduce the above copyright notice,
**   this list of conditions and the following disclaimer
**   in the documentation and/or other materials provided with the distribution.
** * Neither the name of the copyright holder nor the names of its contributors
**   may be used to endorse or promote products derived from this software
**   without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
** THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
** FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
** OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS;
** OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
** EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
*******************************************************************************/

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    allowedOrientations : Orientation.All
    backgroundColor     : "#359cbb"

    PageBusyIndicator {
        anchors {
            top             : weatherInfo.bottom
            horizontalCenter: parent.horizontalCenter
            topMargin       : Theme.dp(30)
        }
        size    : BusyIndicatorSize.Medium
        running : UILink.busyIndicatorActive
    }

    IconButton {
        id : searchButton
        anchors {
            top         : parent.top
            right       : parent.right
            topMargin   : Theme.dp(16)
            rightMargin : Theme.dp(16)
        }
        icon.source : "qrc:///icons/search.svg"
        icon.color  : "white"
        onClicked: {
            UILink.searchCitiesModel.clear();
            pageStack.push(Qt.resolvedUrl("CitySearchPage.qml"));
        }
    }

    Column {
        id              : weatherInfo
        anchors.centerIn: parent

        Row {
            spacing : Theme.dp(50)

            Column {
                spacing: Theme.dp(10)

                Label {
                    text            : UILink.weatherTemperature + "°"
                    font.pointSize  : Theme.fontSizeHuge
                    font.pixelSize  : 0
                    color           : "white"
                }

                Label {
                    text            : UILink.cityName
                    font.pixelSize  : Theme.fontSizeLarge
                    color           : "white"
                }

            }

            Image {
                source: UILink.weatherType === "" ? ""
                                                 : "qrc:///icons/" + UILink.weatherType + ".svg"
                sourceSize {
                    width: Theme.dp(180)
                    height: Theme.dp(180)
                }
            }
        }

        Column {
            spacing: Theme.dp(10)
            Label {
                text            : "Ощущается как " + UILink.weatherFeelsLike + "°"
                font.pixelSize  : Theme.fontSizeSmall
                color           : "white"
            }

            Label {
                text            : "Влажность " + UILink.weatherHumidity + "%"
                font.pixelSize  : Theme.fontSizeSmall
                color           : "white"
            }
        }
    }

    Row {
        id : bottomButtonsPanel
        anchors {
            bottom      : parent.bottom
            right       : parent.right
            bottomMargin: Theme.dp(16)
            rightMargin : Theme.dp(16)
        }

        IconButton {
            icon.source: UILink.cityIsFavorite ? "image://theme/icon-m-favorite-selected"
                                               : "image://theme/icon-m-favorite"
            icon.color: "white"
            onClicked: {
                if (UILink.cityIsFavorite)
                    UILink.deleteCurrentCityFromFavorites();
                else
                    UILink.addCurrentCityToFavorites();
            }
        }

        Button {
            text    : "Избранные города"
            color   : "white"
            onClicked: {
                UILink.favoritesCitiesModel.refresh();
                pageStack.push(Qt.resolvedUrl("FavoritesCitiesPage.qml"))
            }
        }
    }

}
