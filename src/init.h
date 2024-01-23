#ifndef INIT_H
#define INIT_H

#include <QObject>
#include <QtQuick>

class ApiController;
class AppDb;
class DBController;
class UIController;

class Init : public QObject
{
    Q_OBJECT
public:
    explicit Init(QObject *parent = nullptr);

    void startApp();

private:
    QSharedPointer<QQuickView> m_view;
    QSharedPointer<ApiController> m_apiController;
    QSharedPointer<AppDb> m_appDb;
    QSharedPointer<DBController> m_dbController;
    QSharedPointer<UIController> m_uiController;

};

#endif // INIT_H
