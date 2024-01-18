#include "uicontroller.h"

UIController::UIController(QObject *parent) : QObject(parent)
{
    m_cityTemperature = "0°";
}

QString UIController::getCityTemperature()
{
    return m_cityTemperature;
}

void UIController::setCityTemperature(const QString &temperature)
{
    m_cityTemperature = temperature;
    emit cityTemperatureChanged();
}
