#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>

struct ApiReply {
    int httpCode;
    QString body;

    void clear() {
        httpCode = 0;
        body.clear();
    }
};

struct ApiWeatherInfo {
    QString weatherType;
    QString cityName;
    double longitude = 0.0;
    double latitude = 0.0;
    double temperature = 0.0;
    double feelsLike = 0.0;
    int humidity = 0;
};

struct FavoriteCityInfo {
    QString name;
    double longitude = 0.0;
    double latitude = 0.0;
};

struct CachedCityInfo {
    QString name;
    double longitude = 0.0;
    double latitude = 0.0;
    QString lastWeatherType;
    QString lastTemperature;
    QString lastFeelsLike;
    int lastHumidity = 0;
};

#endif // STRUCTS_H
