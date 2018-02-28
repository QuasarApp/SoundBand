#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QObject>

class PlayListModel : public QObject
{
    Q_OBJECT
public:
    explicit PlayListModel(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PLAYLISTMODEL_H