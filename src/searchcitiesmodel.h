#ifndef SEARCHCITIESMODEL_H
#define SEARCHCITIESMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>

class AppDb;

class SearchCitiesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ModelRoles {
        CityNameRole = Qt::UserRole + 1,
        LongitudeRole,
        LatitudeRole,
        IsFavorite
    };

    explicit SearchCitiesModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setAppDb(QSharedPointer<AppDb> appDb);

public slots:
    void clear();
    void refresh(QString cityName = "");

private:
    struct CityInfoItem {
        QString name;
        double longitude = 0.0;
        double latitude = 0.0;
        bool isFavorite = false;
    };

    QSharedPointer<AppDb> m_appDb;
    QList<CityInfoItem> m_items;
};

#endif // SEARCHCITIESMODEL_H
