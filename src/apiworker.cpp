#include "apiworker.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

ApiWorker::ApiWorker(QObject *parent) : QObject(parent)
{

}

void ApiWorker::getCurrentWeatherData()
{
    QString params = "?lat=54.73&lon=55.96&units=metric";
    ApiReply reply;
    requestApi("/weather", params, reply);
    emit currentWeatherDataChanged(reply);
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
