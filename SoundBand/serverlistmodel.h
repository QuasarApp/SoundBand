#ifndef SERVERLISTMODEL_H
#define SERVERLISTMODEL_H

#include <QObject>

class ServerListModel : public QObject
{
    Q_OBJECT
public:
    explicit ServerListModel(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SERVERLISTMODEL_H