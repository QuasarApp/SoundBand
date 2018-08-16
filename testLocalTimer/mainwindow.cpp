#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    timer.setInterval(0);
    color = Qt::white;
    connect(&timer, &QTimer::timeout, this, &MainWindow::handleTick);
    timer.start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleTick() {

    auto time = QDateTime::currentMSecsSinceEpoch();

    ui->msec->setText(QString::number( time% 1000));

    if (!(time % 1000)) {
        if (color == Qt::white) {
            color = Qt::green;
        } else {
            color = Qt::white;
        }

        ui->color->setStyleSheet(QString("background-color: '%0';").arg(color.name(QColor::HexRgb)));
    }
}
