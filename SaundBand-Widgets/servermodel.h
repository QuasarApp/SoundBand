#ifndef SERVERMODEL_H
#define SERVERMODEL_H
#include <QStandardItemModel>
#include "../sync/sync.h"

class ServerModel :public QStandardItemModel
{
private:
    const QList<ETcpSocket*> * source;

public:
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;

    ServerModel(QList<ETcpSocket *> *newSource = nullptr);
    void setSource(const QList<ETcpSocket*> *newSource);
    void refresh();
    ~ServerModel();
};

#endif // SERVERMODEL_H
