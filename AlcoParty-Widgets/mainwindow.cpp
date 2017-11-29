#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    source = new syncLib::Sync();
    ui->horizontalSlider_Value->setMaximum(100);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete source;
}

void MainWindow::on_Play_clicked()
{
    QString song_path = QFileDialog::getOpenFileName(this);

    if(!song_path.isEmpty() && !source->play(song_path)){
        QMessageBox::critical(this,tr("Error"),tr("Сould not play the file you selected."));
    }
}

void MainWindow::on_Pause_clicked()
{
    source->pause();
}

void MainWindow::on_Stop_clicked()
{
    source->stop();
}

void MainWindow::on_Search_clicked()
{
    if(source->getServersList().isEmpty())
        source->scan();
    else{
        ETcpSocket * server = source->getServersList().front();
        if(!source->listen(server)){
            QMessageBox::critical(this,tr("Error"),tr("Сould not listen the finded server - %0").arg(server->name()));
        }
    }

}

void MainWindow::on_horizontalSlider_Value_valueChanged(int value)
{
//    source->set>setMaximum(100);
}
