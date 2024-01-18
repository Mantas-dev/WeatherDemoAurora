#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QObject>

class UIController : public QObject
{
    Q_PROPERTY(QString cityTemperature READ getCityTemperature WRITE setCityTemperature NOTIFY cityTemperatureChanged)

    Q_OBJECT
public:
    explicit UIController(QObject *parent = nullptr);

    QString getCityTemperature();

    void setCityTemperature(const QString &temperature);

signals:
    void cityTemperatureChanged();

private:
    QString m_cityTemperature;
};

#endif // UICONTROLLER_H
