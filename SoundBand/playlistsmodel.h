#ifndef PLAYLISTSMODEL_H
#define PLAYLISTSMODEL_H

#include <QObject>

class PlayListsModel : public QObject
{
    Q_OBJECT
public:
    explicit PlayListsModel(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PLAYLISTSMODEL_H