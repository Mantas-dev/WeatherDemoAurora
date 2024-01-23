#include "searchcitiesmodel.h"
#include "appdb.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

SearchCitiesModel::SearchCitiesModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

QVariant SearchCitiesModel::data(const QModelIndex &index, int role) const
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
    case IsFavorite:
        ret = cityInfo.isFavorite;
        break;
    }

    return ret;
}

int SearchCitiesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.size();
}

QHash<int, QByteArray> SearchCitiesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CityNameRole] = "cityName";
    roles[LatitudeRole] = "latitude";
    roles[LongitudeRole] = "longitude";
    roles[IsFavorite] = "isFavorite";
    return roles;
}

bool SearchCitiesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    switch (role) {
    case IsFavorite: {
        auto item = &m_items[index.row()];
        if (value.toBool()) {
            FavoriteCityInfo info;
            info.name = item->name;
            info.latitude = item->latitude;
            info.longitude = item->longitude;
            m_appDb->addCityToFavorites(info);
        } else {
            m_appDb->deleteCityFromFavorites(item->name);
        }
        item->isFavorite = value.toBool();
        break;
    }
    default:
        return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

Qt::ItemFlags SearchCitiesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

void SearchCitiesModel::setAppDb(QSharedPointer<AppDb> appDb)
{
    m_appDb = appDb;
}

void SearchCitiesModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
}

void SearchCitiesModel::refresh(QString cityName)
{
    const auto citiesJDoc = QJsonDocument::fromJson(m_appDb->findCities(cityName).toUtf8());

    const auto citiesJArr = citiesJDoc.array();
    beginResetModel();
    m_items.clear();

    for (const auto &cityJVal : citiesJArr) {
        CityInfoItem info;
        const auto cityJObj = cityJVal.toObject();

        info.name = cityJObj.value("city_name").toVariant().toString();
        info.latitude = cityJObj.value("latitude").toVariant().toDouble();
        info.longitude = cityJObj.value("longitude").toVariant().toDouble();

        info.isFavorite = cityJObj.value("is_favorite").toVariant().toBool();

        m_items.append(info);
    }

    endResetModel();
}
