#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QObject>
#include <QSharedPointer>

#include "structs.h"

class AppDb;
class DBController;
class SearchCitiesModel;
class FavoritesCitiesModel;

class UIController : public QObject
{
    Q_PROPERTY(QString cityName READ getCityName WRITE setCityName NOTIFY cityNameChanged)
    Q_PROPERTY(bool    cityIsFavorite READ getCityIsFavorite WRITE setCityIsFavorite NOTIFY cityIsFavoriteChanged)
    Q_PROPERTY(QString weatherTemperature READ getWeatherTemperature WRITE setWeatherTemperature NOTIFY weatherTemperatureChanged)
    Q_PROPERTY(QString weatherType READ getWeatherType WRITE setWeatherType NOTIFY weatherTypeChanged)
    Q_PROPERTY(QString weatherFeelsLike READ getWeatherFeelsLike WRITE setWeatherFeelsLike NOTIFY weatherFeelsLikeChanged)
    Q_PROPERTY(QString weatherHumidity READ getWeatherHumidity WRITE setWeatherHumidity NOTIFY weatherHumidityChanged)
    Q_PROPERTY(bool    busyIndicatorActive READ getBusyIndicatorActive WRITE setBusyIndicatorActive NOTIFY busyIndicatorActiveChanged)

    Q_PROPERTY(QObject* searchCitiesModel READ oSearchCitiesModel CONSTANT)
    Q_PROPERTY(QObject* favoritesCitiesModel READ oFavoritesCitiesModel CONSTANT)

    Q_OBJECT
public:
    explicit UIController(QObject *parent = nullptr);

    QString getCityName();
    bool    getCityIsFavorite();
    QString getWeatherTemperature();
    QString getWeatherType();
    QString getWeatherFeelsLike();
    QString getWeatherHumidity();
    bool    getBusyIndicatorActive();

    void setCityName(const QString &name);
    void setCityIsFavorite(const bool &isFavorite);
    void setCityLatitude(const double &latitude);
    void setCityLongitude(const double &longitude);
    void setWeatherTemperature(const QString &temperature);
    void setWeatherType(const QString &type);
    void setWeatherFeelsLike(const QString &feelsLike);
    void setWeatherHumidity(const QString &humidity);
    void setBusyIndicatorActive(const bool &active);

    void setAppDb(QSharedPointer<AppDb> appDb);
    void updateWeatherUI(const ApiWeatherInfo &info);
    void cacheCityInfo();
    void loadCachedCityInfo(const double &latitude, const double &longitude);

signals:
    void cityNameChanged();
    void cityIsFavoriteChanged();
    void weatherTemperatureChanged();
    void weatherTypeChanged();
    void weatherFeelsLikeChanged();
    void weatherHumidityChanged();
    void busyIndicatorActiveChanged();

    void requestShowWeatherInfo(const double &latitude, const double &longitude);

public slots:
    void addCurrentCityToFavorites();
    void deleteCurrentCityFromFavorites();

private:
    QObject *oSearchCitiesModel() const;
    QObject *oFavoritesCitiesModel() const;

    bool m_cityIsFavorite = false, m_busyIndicatorActive = false;
    double m_cityLongitude = 0.0, m_cityLatitude = 0.0;
    QString m_cityName, m_weatherTemperature, m_weatherType,
            m_weatherFeelsLike, m_weatherHumidity;
    QSharedPointer<AppDb> m_appDb;
    QSharedPointer<SearchCitiesModel> m_searchCitiesModel;
    QSharedPointer<FavoritesCitiesModel> m_favoritesCitiesModel;
};

#endif // UICONTROLLER_H
