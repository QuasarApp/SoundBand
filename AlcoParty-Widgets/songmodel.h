#ifndef SONGMODEL_H
#define SONGMODEL_H
#include <QStandardItemModel>
#include "../sync/sync.h"

class SongModel :public QStandardItemModel
{
private:
    const QList<syncLib::SongHeader> * source;

public:
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;

    SongModel(QList<syncLib::SongHeader> * newSource = nullptr);
    void setSource(const QList<syncLib::SongHeader> *newSource);
    void refresh();
    ~SongModel();
};

#endif // SONGMODEL_H
