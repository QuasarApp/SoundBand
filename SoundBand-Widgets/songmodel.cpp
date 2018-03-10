#include "songmodel.h"

SongModel::SongModel(QList<syncLib::SongHeader> *newSource):
    source(newSource)
{
    this->setHorizontalHeaderLabels(QStringList() << tr("Id") << tr("Name") << tr("Size"));
    if(newSource)
        this->setRowCount(newSource->size());
}

bool SongModel::setData(const QModelIndex &, const QVariant& , int){
    setRowCount(source->size());
    return false;
}

QVariant SongModel::data(const QModelIndex &index, int role) const{
    if(!source) return QVariant();

    QList<syncLib::SongHeader>::const_iterator idItem=source->begin()+index.row();

    switch (role) {
    case Qt::DisplayRole:{
        switch(index.column()){
        case 0: return idItem->id;
        case 1: return idItem->name;
        case 2: return QString("%0 Mb").arg(idItem->size / 1000000.0);
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

void SongModel::setSource(const QList<syncLib::SongHeader> *newSource){

    source = newSource;
    setRowCount(source->size());

}

void SongModel::refresh(){
    if(source&&source->size()!=this->rowCount())
        setRowCount(source->size());

}

SongModel::~SongModel(){

}
