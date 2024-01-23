#ifndef APIWORKER_H
#define APIWORKER_H

#include <QObject>

#include "structs.h"

class ApiWorker : public QObject
{
    Q_OBJECT
public:
    explicit ApiWorker(QObject *parent = nullptr);

signals:
    void currentWeatherDataChanged(ApiWeatherInfo reply);

public slots:
    void getCurrentWeatherData(const double &latitude, const double &longitude);

private:
    bool requestApi(const QString &endpoint, const QString &params, ApiReply &apiReply);
    QString parseWeatherType(const QJsonObject &object);

};

#endif // APIWORKER_H
