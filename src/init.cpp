#include "init.h"
#include "apicontroller.h"
#include "appdb.h"
#include "dbcontroller.h"
#include "uicontroller.h"

#include <auroraapp.h>
#include <random>

Init::Init(QObject *parent) :
    QObject(parent), m_view(Aurora::Application::createView()),
    m_apiController(new ApiController), m_appDb(new AppDb),
    m_dbController(new DBController), m_uiController(new UIController)
{
    qRegisterMetaType<ApiWeatherInfo>("ApiWeatherInfo");

    connect(m_apiController.data(), &ApiController::currentWeatherDataChanged,
            this, [&](ApiWeatherInfo reply){
        m_uiController->setBusyIndicatorActive(false);
        m_uiController->updateWeatherUI(reply);
    });
    connect(m_uiController.data(), &UIController::requestShowWeatherInfo,
            this, [&](const double &latitude, const double &longitude){
        m_uiController->setBusyIndicatorActive(true);
        m_uiController->loadCachedCityInfo(latitude, longitude);
        emit m_apiController->requestCurrentWeatherData(latitude, longitude);
    });
}

void Init::startApp()
{
    m_view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/WeatherDemo.qml")));
    m_view->rootContext()->setContextProperty("UILink", m_uiController.data());
    m_view->show();


    m_dbController->init();
    m_appDb->setDbController(m_dbController.data());
    m_uiController->setAppDb(m_appDb);

    auto latitude = m_appDb->getParam("geo.latitude").toDouble();
    auto longitude = m_appDb->getParam("geo.longitude").toDouble();

    QVector<QPair<double, double>> coords{{55.76, 37.62}, {53.18, 50.12}, {54.77, 56.03}, {47.24, 39.71},
                                          {56.33, 44.01}, {58.53, 31.28}, {55.79, 49.11}};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, coords.size() - 1);
    auto cityCoords = coords[dist(gen)];

    if (!qFuzzyCompare(latitude, cityCoords.first) || !qFuzzyCompare(longitude, cityCoords.second)) {
        latitude = cityCoords.first;
        longitude = cityCoords.second;
        m_appDb->setParam("geo.latitude", QString::number(latitude));
        m_appDb->setParam("geo.longitude", QString::number(longitude));
    }

    emit m_uiController->requestShowWeatherInfo(latitude, longitude);
}
