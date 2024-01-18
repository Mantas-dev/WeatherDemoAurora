#include "init.h"
#include <auroraapp.h>
#include "apicontroller.h"
#include "uicontroller.h"

Init::Init(QObject *parent) :
    QObject(parent), m_view(Aurora::Application::createView()),
    m_apiController(new ApiController), m_uiController(new UIController)
{
    qRegisterMetaType<ApiReply>("ApiReply");
}

void Init::startApp()
{
    m_view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/WeatherDemo.qml")));
    m_view->rootContext()->setContextProperty("UILink", m_uiController.data());
    m_view->show();
}
