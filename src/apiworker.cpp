#include "apiworker.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

ApiWorker::ApiWorker(QObject *parent) : QObject(parent)
{

}

void ApiWorker::getCurrentWeatherData(const double &latitude, const double &longitude)
{
    QString params = QString("?lat=%1&lon=%2&units=metric&lang=ru").arg(latitude).arg(longitude);
    ApiReply apiReply;
    ApiWeatherInfo weatherInfo;
    requestApi("/weather", params, apiReply);

    if (apiReply.httpCode == 200) {
        const auto jObj = QJsonDocument::fromJson(apiReply.body.toUtf8()).object();
        const auto coordJObj = jObj.value("coord").toObject();
        const auto weatherJArr = jObj.value("weather").toArray();
        const auto mainJObj = jObj.value("main").toObject();

        weatherInfo.cityName = jObj.value("name").toVariant().toString();
        weatherInfo.latitude = coordJObj.value("lat").toVariant().toDouble();
        weatherInfo.longitude = coordJObj.value("lon").toVariant().toDouble();
        weatherInfo.weatherType = parseWeatherType(weatherJArr.first().toObject());
        weatherInfo.temperature = mainJObj.value("temp").toVariant().toDouble();
        weatherInfo.feelsLike = mainJObj.value("feels_like").toVariant().toDouble();
        weatherInfo.humidity = mainJObj.value("humidity").toVariant().toDouble();
    }

    emit currentWeatherDataChanged(weatherInfo);
}

bool ApiWorker::requestApi(const QString &endpoint, const QString &params, ApiReply &apiReply)
{
    QString appId = "d3b4ff6d865fd930383924a81911b96a";
    QString url = "https://api.openweathermap.org/data/2.5" + endpoint + params;

    if (params.isEmpty())
        url.append('?');
    else
        url.append('&');

    url.append("appid=" + appId);

    QScopedPointer<QNetworkAccessManager> netManager(new QNetworkAccessManager());

    QEventLoop eventLoop;
    QTimer timer;
    connect(netManager.data(), &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
    conf.setProtocol(QSsl::AnyProtocol);
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    timer.setSingleShot(true);
    timer.start(20000);

    QScopedPointer<QNetworkReply> reply(netManager->get(request));
    eventLoop.exec();

    qDebug() << "### url - " << url;

    if (timer.isActive()) {
        timer.stop();
    } else {
        qCritical() << "Network timeout!";
        return false;
    }

    QByteArray answer = reply->readAll();
    int replyError = (int)reply->error();

    qDebug() << "### answer - " << answer;

    // Ошибка соединения
    if (answer.isEmpty() || (replyError >= 1 && replyError <= 99)) {
        qCritical() << "Network request error code:" << reply->error() << reply->errorString();
        qCritical() << "URL:" << url;
        qCritical() << "Answer:" << answer;
        return false;
    }

    apiReply.httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(apiReply.httpCode != 200) {
        qInfo() << "HTTP reply code:" << apiReply.httpCode;
        qInfo() << "HTTP repl:" << answer;
    }

    apiReply.body = answer;

    return true;
}

QString ApiWorker::parseWeatherType(const QJsonObject &object)
{
    QString weatherType = object.value("main").toVariant().toString();
    const auto typeId = object.value("id").toVariant().toInt();

    if (typeId >= 700 && typeId < 800)
        weatherType = "atmosphere";

    return weatherType.toLower();
}
