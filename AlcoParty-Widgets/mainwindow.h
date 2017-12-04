#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../sync/sync.h"
#include "songmodel.h"
#include "servermodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_Play_clicked();

    void on_Pause_clicked();

    void on_Stop_clicked();

    void on_Search_clicked();

    void on_horizontalSlider_Value_valueChanged(int value);

    void on_listen_clicked();

    void on_Select_clicked();

    void on_horizontalSlider_Seek_sliderReleased();

    void on_seekChanged(qint64 value);

    void on_network_state_changed();

private:
    syncLib::Sync *source;
    const QList<ETcpSocket *> *playList;
    SongModel songModel;
    ServerModel serverModel;
//    QList<syncLib::SongHeader>* playList;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
