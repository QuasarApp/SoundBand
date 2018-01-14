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
    ui->horizontalSlider_Value->setValue(source->getValume());

    songModel.setSource(source->getPlayList());
    playList = &(source->getServersList());
    serverModel.setSource(playList);

    ui->localSongsView->setModel(&songModel);
    ui->ServersView->setModel(&serverModel);

    ui->localSongsView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->ServersView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    ui->horizontalSlider_Seek->setMaximum(1000000);

    connect(source,SIGNAL(seekChanged(qint64)),SLOT(on_seekChanged(qint64)));
    connect(source,SIGNAL(networkStateChange()),SLOT(on_network_state_changed()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete source;
}

void MainWindow::on_Play_clicked()
{

    int curentSong = ui->localSongsView->currentIndex().row();
    if(!source->play(songModel.data(songModel.index(curentSong, 0), Qt::DisplayRole).toInt())){
          QMessageBox::critical(this,tr("Error"),tr("Сould not play the file you selected."));
    }

}

void MainWindow::on_Pause_clicked()
{
    if(ui->Pause->text() == "Play"){
        source->pause(false);
        ui->Pause->setText("Pause");

    }else{
        source->pause(true);
        ui->Pause->setText("Play");

    }
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
            QMessageBox::critical(this,tr("Error"),tr("Сould not listen the finded server - %0").arg(server->peerName()));
        }
    }

}

void MainWindow::on_horizontalSlider_Value_valueChanged(int value)
{
    source->setValume(value);
}

void MainWindow::on_listen_clicked()
{
    int curentServer = ui->ServersView->currentIndex().row();
    if(!source->listen((*playList)[curentServer])){
          QMessageBox::critical(this,tr("Error"),tr("Сould not play the file you selected."));
    }
}

void MainWindow::on_Select_clicked()
{
    QStringList songs =  QFileDialog::getOpenFileNames(this);

    for(QString url:songs){
        source->addNewSong(url);
        songModel.refresh();
    }
}

void MainWindow::on_horizontalSlider_Seek_sliderReleased(){
    source->jump(ui->horizontalSlider_Seek->value() / 1000000.0 *  source->getEndPoint());
}

void MainWindow::on_seekChanged(qint64 value){
    ui->horizontalSlider_Seek->setValue(value * 1000000 / source->getEndPoint());
}

void MainWindow::on_network_state_changed(){
    serverModel.refresh();
}
