#include "servermodel.h"

#include <QHostInfo>
ServerModel::ServerModel(QList<ETcpSocket *> *newSource):
    source(newSource)
{
    this->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("IP"));
    if(newSource)
        this->setRowCount(newSource->size());
}

bool ServerModel::setData(const QModelIndex &, const QVariant& , int){
    setRowCount(source->size());
    return false;
}

QVariant ServerModel::data(const QModelIndex &index, int role) const{
    if(!source) return QVariant();

    QList<ETcpSocket *>::const_iterator idItem=source->begin()+index.row();

    switch (role) {
    case Qt::DisplayRole:{
        switch (index.column()) {
        case 1:
            return  (*idItem)->name();
        default:
            return "no suported";

        }

    }

    case Qt::TextAlignmentRole:{
        return Qt::AlignCenter;
    }

    case Qt::TextColorRole:
        return QColor(20,20,25);

    default:
        break;
    }
    return QVariant();
}

void ServerModel::setSource(const QList<ETcpSocket *> *newSource){

    source = newSource;
    setRowCount(source->size());

}

void ServerModel::refresh(){
    if(source&&source->size()!=this->rowCount())
        setRowCount(source->size());

}

ServerModel::~ServerModel(){

}
