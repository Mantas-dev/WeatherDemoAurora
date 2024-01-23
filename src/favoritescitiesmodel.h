#ifndef FAVORITESCITIESMODEL_H
#define FAVORITESCITIESMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>

#include "structs.h"

class AppDb;

class FavoritesCitiesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ModelRoles {
        CityNameRole = Qt::UserRole + 1,
        LongitudeRole,
        LatitudeRole
    };

    explicit FavoritesCitiesModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setAppDb(QSharedPointer<AppDb> appDb);

public slots:
    void refresh();
    void remove(const int &index);

private:
    QSharedPointer<AppDb> m_appDb;
    QList<FavoriteCityInfo> m_items;
};

#endif // FAVORITESCITIESMODEL_H
