#ifndef APPDB_H
#define APPDB_H

#include <QObject>
#include <QSharedPointer>

#include "structs.h"

class DBController;

class AppDb : public QObject
{
    Q_OBJECT
public:
    explicit AppDb(QObject *parent = nullptr);

    void setDbController(DBController *controller);

    bool cityIsFavorite(const QString &cityName);
    QString findCities(QString cityName = "");
    QString getFavoritesCities();
    QString getParam(const QString &paramName);
    CachedCityInfo getCachedCityInfo(const double &latitude, const double &longitude);

    void setParam(const QString &paramName, const QString &paramValue);
    void addCityToFavorites(const FavoriteCityInfo &info);
    void deleteCityFromFavorites(const QString &cityName);
    void cacheCity(const CachedCityInfo &info);

private:
    QSharedPointer<DBController> m_db;

};

#endif // APPDB_H
