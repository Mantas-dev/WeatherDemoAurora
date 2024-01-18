#include "apicontroller.h"
#include "apiworker.h"

ApiController::ApiController(QObject *parent) :
    QObject(parent), m_workerThread(new QThread), m_apiWorker(new ApiWorker)
{
    m_apiWorker->moveToThread(m_workerThread.data());

    //controller -> worker
    connect(this, &ApiController::requestCurrentWeatherData, m_apiWorker.data(), &ApiWorker::getCurrentWeatherData);

    //worker -> controller
    connect(m_apiWorker.data(), &ApiWorker::currentWeatherDataChanged, this, &ApiController::currentWeatherDataChanged);

    connect(m_workerThread.data(), &QThread::finished, m_apiWorker.data(), &QObject::deleteLater);
    m_workerThread->start();
}

ApiController::~ApiController()
{
    m_workerThread->quit();
    m_workerThread->wait();
}
