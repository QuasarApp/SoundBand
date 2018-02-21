#include "filedialog.h"
#include <QEventLoop>
#include <QTimer>

#ifdef QT_DEBUG
#include <QDebug>
#endif

FileDialog::FileDialog(QObject *ptr):
    QObject(ptr)
{
#ifdef Q_OS_ANDROID
    dialog = new AndroidFileDialog(this);
#endif

}

void FileDialog::done(QString s){
    _fileUrl = s;
    if(!_fileUrl.isEmpty())
        emit fileUrlDone();
}

void FileDialog::open(){
#ifdef Q_OS_ANDROID
    connect(dialog,SIGNAL(existingFileNameReady(QString)),SLOT(done(QString)));
    bool success = dialog->provideExistingFileName();
    if (!success) {
#ifdef QT_DEBUG
        qDebug() << "Problem with JNI or sth like that...";
#endif
        disconnect(dialog, SIGNAL(existingFileNameReady(QString)), this, SLOT(done(QString)));
        //or just delete fileDialog instead of disconnect
    }

#else
    _filesUrls = QFileDialog::getOpenFileNames(nullptr, _title, _dir, _filter);
    if(!_filesUrls.isEmpty())
        emit filesUrlsDone();

#endif
}

QString FileDialog::save(const QString &name){
    return QDir::homePath() + "/SoundBandSongs/" +name;
}

QString FileDialog::title() const{
    return _title;
}

QString FileDialog::dir() const{
    return _dir;
}

QString FileDialog::filter() const{
    return _filter;
}

QStringList FileDialog::filesUrls() const{
    return _filesUrls;
}

QString FileDialog::fileUrl() const{
    return _fileUrl;
}

void FileDialog::setTitle(const QString &title){
    _title = title;
}

void FileDialog::setDir(const QString &dir){
    _dir = dir;
}

void FileDialog::setFilter(const QString &filter){
    _filter = filter;
}

FileDialog::~FileDialog(){

}
