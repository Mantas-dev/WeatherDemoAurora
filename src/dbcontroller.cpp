#include "dbcontroller.h"

#include <QFile>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QtConcurrent>

DBController::DBController(QObject *parent) : QObject(parent)
{

}

void DBController::init()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "DBController");

    if (!db.isValid()) {
        qCritical() << "Cannot create SQLITE database";
        return;
    }

    db.setDatabaseName("WeatherDB.db");

    if (!db.open()) {
        qCritical() << "Cannot open database file";
        return;
    }

    qInfo() << "Database is opened";

    if (db.tables(QSql::Tables).isEmpty()) {
        qInfo() << "Database is empty. Data will be prepared";
        if (createTables()) {
            qInfo() << "Tables created. Loading JSON data";
            QtConcurrent::run([&](){ loadJsonData(); });
        }
    }
}

bool DBController::execSql(const QString &query)
{
    QSqlQuery sqlQuery(db);
    bool ret = sqlQuery.exec(query);

    if (!ret) {
        qCritical() << "SQL query error:" << query;
        qCritical() << sqlQuery.lastError().text();
    }

    return ret;
}

bool DBController::execFile(const QString &filePath)
{
    QFile file(filePath);

    if (!file.exists() || !file.open(QIODevice::ReadOnly))
        return false;

    QList<QString> queries;

    auto data = QString(file.readAll()).split("\n");
    file.close();

    QString query;
    bool finishedQuery = false;
    for (const auto &line : data) {
        auto trimmedLine = line.trimmed();

        if (trimmedLine.isEmpty() || trimmedLine.indexOf("--") == 0)
            continue;

        query += trimmedLine + " ";

        if (trimmedLine.right(1) == ";")
            finishedQuery = true;

        if (finishedQuery) {
            if (query.trimmed().right(1) != ";")
                query.append(" ;");
            queries.append(query);
            query.clear();
            finishedQuery = false;
        }

    }

    return execSqlList(queries);
}

bool DBController::execSqlList(const QList<QString> &queries)
{
    bool ret = false;

    if (db.isOpen()) {
        db.transaction();
        QSqlQuery sqlQuery(db);

        for (const auto &query : queries) {
            ret = sqlQuery.exec(query);
            if (!ret) {
                qCritical() << "SQL query error:" << query;
                qCritical() << sqlQuery.lastError().text();
                break;
            }
        }

        if (ret)
            ret = db.commit();

        if (!ret)
            db.rollback();
    }

    return ret;
}

bool DBController::createTables()
{
    return execFile(":/tables.sql");
}

void DBController::loadJsonData()
{
    QFile file(":/russian-cities.json");

    if (!file.exists() || !file.open(QIODevice::ReadOnly))
        return;

    auto jArr = QJsonDocument::fromJson(file.readAll()).array();

    auto startMs = QDateTime::currentMSecsSinceEpoch();

    for (const auto &jVal : jArr) {
        const auto jObj = jVal.toObject();
        const auto coords = jObj.value("coords").toObject();
        const auto cityName = jObj.value("name").toVariant().toString();
        const auto latitude = coords.value("lat").toVariant().toDouble();
        const auto longitude = coords.value("lon").toVariant().toDouble();

        QString query = QString("INSERT INTO cities(city_name,latitude,longitude) VALUES('%1', %2, %3) \n"
                                "    ON CONFLICT(city_name) DO UPDATE SET latitude=excluded.latitude, longitude=excluded.longitude;")
                                .arg(cityName).arg(latitude).arg(longitude);
        execSql(query);
    }
    auto endMs = QDateTime::currentMSecsSinceEpoch();

    qInfo() << QString("JSON data loaded (%1 ms)").arg(endMs - startMs);
}

QString DBController::querySQLN(const QString &query)
{
    QJsonArray ret;
    QSqlQuery sqlQuery(db);
    sqlQuery.setForwardOnly(true);

    if (!sqlQuery.exec(query)) {
        qCritical() << "SQL query error:" << query;
        qCritical() << sqlQuery.lastError().text();
        return "";
    }

    QSqlRecord fields = sqlQuery.record();

    while (sqlQuery.next()) {
        QVariantMap line;
        for (int i = 0; i < fields.count(); ++i) {
            if (sqlQuery.value(i).isNull())
                line.insert(fields.fieldName(i), "");
            else
                line.insert(fields.fieldName(i), sqlQuery.value(i).toString());
        }
        ret.append(QJsonValue(QJsonObject::fromVariantMap(line)));
    }
    return QJsonDocument(ret).toJson();
}

QHash<QString, QString> DBController::querySQLOneN(const QString &query)
{
    QHash<QString, QString> ret;
    QSqlQuery sqlQuery(db);
    sqlQuery.setForwardOnly(true);

    if (!sqlQuery.exec(query)) {
        qCritical() << "SQL query error:" << query;
        qCritical() << sqlQuery.lastError().text();
    } else {
        QSqlRecord fields = sqlQuery.record();
        if (sqlQuery.next()) {
            ret.clear();
            for (int i = 0; i < fields.count(); ++i) {
                if (sqlQuery.value(i).isNull())
                    ret.insert(fields.fieldName(i), "");
                else
                    ret.insert(fields.fieldName(i), sqlQuery.value(i).toString());
            }
        }
    }

    return ret;
}
