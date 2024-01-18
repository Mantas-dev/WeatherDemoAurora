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
    void currentWeatherDataChanged(ApiReply reply);

public slots:
    void getCurrentWeatherData();

private:
    bool requestApi(const QString &endpoint, const QString &params, ApiReply &apiReply);

};

#endif // APIWORKER_H
