#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QDateTime>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    timer.setInterval(0);
    color = Qt::white;
    connect(&timer, &QTimer::timeout, this, &MainWindow::handleTick);
    timer.start();
}

void MainWindow::keyPressEvent(QKeyEvent * key) {

    if (key->key() == Qt::Key_Space) {
        if (timer.isActive()) {
            timer.stop();
        } else {
            timer.start();
        }
    }


}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleTick() {

    auto time = QDateTime::currentMSecsSinceEpoch();

    ui->msec->setText(QString::number(time));
}
