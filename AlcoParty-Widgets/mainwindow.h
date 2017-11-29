#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../sync/sync.h"

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

private:
    syncLib::Sync *source;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
