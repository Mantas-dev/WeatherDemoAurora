#include "appdb.h"
#include "dbcontroller.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>

AppDb::AppDb(QObject *parent) : QObject(parent)
{

}

void AppDb::setDbController(DBController *controller)
{
    m_db.reset(controller);
}

bool AppDb::cityIsFavorite(const QString &cityName)
{
    auto reply = m_db->querySQLOneN("SELECT * FROM favorites_cities AS fc \n"
                                    "WHERE fc.city_name = '" + cityName + "'");
    return !reply.value("city_name").isNull();
}

QString AppDb::findCities(QString cityName)
{
    if (cityName.isEmpty())
        return "";
    QString query = QString("SELECT cities.*, \n"
                            "       (CASE WHEN fc.city_name IS NULL THEN 0 ELSE 1 END) AS is_favorite \n"
                            "FROM cities \n"
                            "LEFT JOIN favorites_cities AS fc \n"
                            "       ON fc.city_name = cities.city_name \n"
                            "WHERE cities.city_name LIKE '" + cityName + "%' \n"
                            "LIMIT 10");

    return m_db->querySQLN(query);
}

QString AppDb::getFavoritesCities()
{
    return m_db->querySQLN("SELECT * FROM favorites_cities ORDER BY city_name");
}

QString AppDb::getParam(const QString &paramName)
{
    return m_db->querySQLOneN("SELECT value FROM params WHERE name='" + paramName + "'").value("name").toString();
}

CachedCityInfo AppDb::getCachedCityInfo(const double &latitude, const double &longitude)
{
    QString query = QString("SELECT * FROM cached_cities \n"
                            "WHERE latitude = %1 AND longitude = %2").arg(latitude).arg(longitude);

    CachedCityInfo info;
    const auto reply = m_db->querySQLOneN(query);

    info.name = reply.value("city_name").toString();
    info.latitude = reply.value("latitude").toDouble();
    info.longitude = reply.value("longitude").toDouble();
    info.lastWeatherType = reply.value("last_weather_type").toString();
    info.lastTemperature = reply.value("last_temperature").toString();
    info.lastFeelsLike = reply.value("last_feels_like").toString();
    info.lastHumidity = reply.value("last_humidity").toInt();

    return info;
}

void AppDb::setParam(const QString &paramName, const QString &paramValue)
{
    QString query = QString("INSERT INTO params \n"
                            "VALUES ('%1', '%2') \n"
                            "ON CONFLICT(name) \n"
                            "DO UPDATE SET value=excluded.value").arg(paramName).arg(paramValue);
    m_db->execSql(query);
}

void AppDb::addCityToFavorites(const FavoriteCityInfo &info)
{
    QString query = QString("INSERT INTO favorites_cities \n"
                            "VALUES ('%1', %2, %3)").arg(info.name).arg(info.latitude).arg(info.longitude);
    m_db->execSql(query);
}

void AppDb::deleteCityFromFavorites(const QString &cityName)
{
    m_db->execSql("DELETE FROM favorites_cities WHERE city_name = '" + cityName + "'");
}

void AppDb::cacheCity(const CachedCityInfo &info)
{
    QString query = QString("INSERT INTO cached_cities \n"
                            "VALUES ('%1', %2, %3, '%4', '%5', '%6', '%7') \n"
                            "ON CONFLICT(city_name) \n"
                            "DO UPDATE SET latitude = excluded.latitude, longitude = excluded.longitude, \n"
                            "              last_weather_type = excluded.last_weather_type, \n"
                            "              last_temperature = excluded.last_temperature, \n"
                            "              last_feels_like = excluded.last_feels_like, \n"
                            "              last_humidity = excluded.last_humidity").arg(info.name)
                                                                                   .arg(info.latitude)
                                                                                   .arg(info.longitude)
                                                                                   .arg(info.lastWeatherType)
                                                                                   .arg(info.lastTemperature)
                                                                                   .arg(info.lastFeelsLike)
                                                                                   .arg(info.lastHumidity);
    m_db->execSql(query);
}
