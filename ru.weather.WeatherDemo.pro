################################################################################
##
## Copyright (C) 2022 ru.weather
## 
## This file is part of the Weather Demo project.
##
## Redistribution and use in source and binary forms,
## with or without modification, are permitted provided
## that the following conditions are met:
##
## * Redistributions of source code must retain the above copyright notice,
##   this list of conditions and the following disclaimer.
## * Redistributions in binary form must reproduce the above copyright notice,
##   this list of conditions and the following disclaimer
##   in the documentation and/or other materials provided with the distribution.
## * Neither the name of the copyright holder nor the names of its contributors
##   may be used to endorse or promote products derived from this software
##   without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
## THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
## FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
## IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
## FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
## OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
## LOSS OF USE, DATA, OR PROFITS;
## OR BUSINESS INTERRUPTION)
## HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
## WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
## (INCLUDING NEGLIGENCE OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
## EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##
################################################################################

TARGET = ru.weather.WeatherDemo

QT += sql network concurrent

CONFIG += \
    auroraapp

PKGCONFIG += \

SOURCES += \
    src/apicontroller.cpp \
    src/apiworker.cpp \
    src/appdb.cpp \
    src/dbcontroller.cpp \
    src/favoritescitiesmodel.cpp \
    src/init.cpp \
    src/main.cpp \
    src/searchcitiesmodel.cpp \
    src/uicontroller.cpp

HEADERS += \
    src/apicontroller.h \
    src/apiworker.h \
    src/appdb.h \
    src/dbcontroller.h \
    src/favoritescitiesmodel.h \
    src/init.h \
    src/searchcitiesmodel.h \
    src/structs.h \
    src/uicontroller.h

DISTFILES += \
    qml/pages/CitySearchPage.qml \
    qml/pages/FavoritesCitiesPage.qml \
    rpm/ru.weather.WeatherDemo.spec \
    CODE_OF_CONDUCT.md \
    CONTRIBUTING.md \
    LICENSE.BSD-3-CLAUSE.md \
    README.md \
    qml/WeatherDemo.qml \
    qml/cover/DefaultCoverPage.qml \
    qml/pages/MainPage.qml \

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.weather.WeatherDemo.ts \
    translations/ru.weather.WeatherDemo-ru.ts \

RESOURCES += \
    source.qrc
