#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <QObject>
#include <QSqlDatabase>

class DBController : public QObject
{
    Q_OBJECT
public:
    explicit DBController(QObject *parent = nullptr);
    void init();
    bool execSql(const QString &query);
    bool execFile(const QString &filePath);
    bool execSqlList(const QList<QString> &queries);

private:
    bool createTables();
    void loadJsonData();
    QString querySQLN(const QString &sql);
    QHash<QString, QString> querySQLOneN(const QString &query);

    QSqlDatabase db;

};

#endif // DBCONTROLLER_H
