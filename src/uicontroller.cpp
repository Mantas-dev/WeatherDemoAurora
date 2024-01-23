#include "uicontroller.h"
#include "searchcitiesmodel.h"
#include "favoritescitiesmodel.h"
#include "appdb.h"

UIController::UIController(QObject *parent)
    : QObject(parent), m_searchCitiesModel(new SearchCitiesModel),
      m_favoritesCitiesModel(new FavoritesCitiesModel)
{

}

QString UIController::getCityName()
{
    return m_cityName;
}

bool UIController::getCityIsFavorite()
{
    return m_cityIsFavorite;
}

QString UIController::getWeatherTemperature()
{
    return m_weatherTemperature;
}

QString UIController::getWeatherType()
{
    return m_weatherType;
}

QString UIController::getWeatherFeelsLike()
{
    return m_weatherFeelsLike;
}

QString UIController::getWeatherHumidity()
{
    return m_weatherHumidity;
}

void UIController::setCityName(const QString &name)
{
    m_cityName = name;
    emit cityNameChanged();
}

void UIController::setCityIsFavorite(const bool &isFavorite)
{
    m_cityIsFavorite = isFavorite;
    emit cityIsFavoriteChanged();
}

void UIController::setCityLatitude(const double &latitude)
{
    m_cityLatitude = latitude;
}

void UIController::setCityLongitude(const double &longitude)
{
    m_cityLongitude = longitude;
}

void UIController::setWeatherTemperature(const QString &temperature)
{
    m_weatherTemperature = temperature;
    emit weatherTemperatureChanged();
}

void UIController::setWeatherType(const QString &type)
{
    m_weatherType = type;
    emit weatherTypeChanged();
}

void UIController::setWeatherFeelsLike(const QString &feelsLike)
{
    m_weatherFeelsLike = feelsLike;
    emit weatherFeelsLikeChanged();
}

void UIController::setWeatherHumidity(const QString &humidity)
{
    m_weatherHumidity = humidity;
    emit weatherHumidityChanged();
}

void UIController::setAppDb(QSharedPointer<AppDb> appDb)
{
    m_appDb = appDb;
    m_searchCitiesModel->setAppDb(appDb);
    m_favoritesCitiesModel->setAppDb(appDb);
}

void UIController::updateWeatherUI(const ApiWeatherInfo &info)
{
    if (!info.cityName.isEmpty()) {
        setCityName(info.cityName);
        setCityLatitude(info.latitude);
        setCityLongitude(info.longitude);
        setWeatherTemperature(QString::number(qRound(info.temperature)));
        setWeatherType(info.weatherType);
        setWeatherFeelsLike(QString::number(qRound(info.feelsLike)));
        setWeatherHumidity(QString::number(info.humidity));
        cacheCityInfo();
    }
}

void UIController::cacheCityInfo()
{
    CachedCityInfo info;
    info.name = m_cityName;
    info.latitude = m_cityLatitude;
    info.longitude = m_cityLongitude;
    info.lastWeatherType = m_weatherType;
    info.lastTemperature = m_weatherTemperature;
    info.lastFeelsLike = m_weatherFeelsLike;
    info.lastHumidity = m_weatherHumidity.toInt();
    m_appDb->cacheCity(info);
}

void UIController::loadCachedCityInfo(const double &latitude, const double &longitude)
{
    const auto info = m_appDb->getCachedCityInfo(latitude, longitude);

    setCityName(info.name);
    setWeatherTemperature(QString::number(qRound(info.lastTemperature.toDouble())));
    setWeatherType(info.lastWeatherType);
    setWeatherFeelsLike(QString::number(qRound(info.lastFeelsLike.toDouble())));
    setWeatherHumidity(QString::number(info.lastHumidity));
    setCityIsFavorite(m_appDb->cityIsFavorite(info.name));
}

void UIController::addCurrentCityToFavorites()
{
    FavoriteCityInfo info;
    info.name = m_cityName;
    info.latitude = m_cityLatitude;
    info.longitude = m_cityLongitude;
    m_appDb->addCityToFavorites(info);
    setCityIsFavorite(true);
}

void UIController::deleteCurrentCityFromFavorites()
{
    m_appDb->deleteCityFromFavorites(m_cityName);
    setCityIsFavorite(false);
}

QObject *UIController::oSearchCitiesModel() const
{
    return m_searchCitiesModel.data();
}

QObject *UIController::oFavoritesCitiesModel() const
{
    return m_favoritesCitiesModel.data();
}
