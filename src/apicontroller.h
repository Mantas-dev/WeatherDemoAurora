#ifndef APICONTROLLER_H
#define APICONTROLLER_H

#include <QObject>
#include <QThread>
#include <QSharedPointer>

#include "structs.h"

class ApiWorker;

class ApiController : public QObject
{
    Q_OBJECT
public:
    explicit ApiController(QObject *parent = nullptr);
    ~ApiController();

signals:
    void requestCurrentWeatherData(const double latitude, const double longitude);

    void currentWeatherDataChanged(ApiWeatherInfo reply);

private:
    QSharedPointer<QThread> m_workerThread;
    QSharedPointer<ApiWorker> m_apiWorker;
};

#endif // APICONTROLLER_H
