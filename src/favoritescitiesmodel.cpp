#include "favoritescitiesmodel.h"
#include "appdb.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

FavoritesCitiesModel::FavoritesCitiesModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

QVariant FavoritesCitiesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_items.size())
        return QVariant();

    QVariant ret;
    const auto cityInfo = m_items.at(index.row());

    switch (role) {
    case CityNameRole:
        ret = cityInfo.name;
        break;
    case LatitudeRole:
        ret = cityInfo.latitude;
        break;
    case LongitudeRole:
        ret = cityInfo.longitude;
        break;
    }

    return ret;
}

int FavoritesCitiesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.size();
}

QHash<int, QByteArray> FavoritesCitiesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CityNameRole] = "cityName";
    roles[LatitudeRole] = "latitude";
    roles[LongitudeRole] = "longitude";
    return roles;
}

void FavoritesCitiesModel::setAppDb(QSharedPointer<AppDb> appDb)
{
    m_appDb = appDb;
}

void FavoritesCitiesModel::refresh()
{
    QJsonParseError errorInfo;
    const auto citiesJDoc = QJsonDocument::fromJson(m_appDb->getFavoritesCities().toUtf8(), &errorInfo);

    const auto citiesJArr = citiesJDoc.array();
    beginResetModel();
    m_items.clear();

    for (const auto &cityJVal : citiesJArr) {
        FavoriteCityInfo info;
        const auto cityJObj = cityJVal.toObject();

        info.name = cityJObj.value("city_name").toVariant().toString();
        info.latitude = cityJObj.value("latitude").toVariant().toDouble();
        info.longitude = cityJObj.value("longitude").toVariant().toDouble();

        m_items.append(info);
    }

    endResetModel();
}

void FavoritesCitiesModel::remove(const int &index)
{
    if (index < 0 || index > m_items.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_appDb->deleteCityFromFavorites(m_items.at(index).name);
    m_items.removeAt(index);
    endRemoveRows();
}
