#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent*);
    ~MainWindow();

private:
    QTimer timer;
    QColor color;
    Ui::MainWindow *ui;
private slots:
    void handleTick();
};

#endif // MAINWINDOW_H
