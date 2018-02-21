#ifndef FILEDIALOG_H
#define FILEDIALOG_H
#include "androidfiledialog.h"

#include <QFileDialog>

/**
 * @brief The FileDialog class general crossplatform dialog window
 */
class FileDialog: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString dir READ dir WRITE setDir)
    Q_PROPERTY(QString filter READ filter WRITE setFilter)
    Q_PROPERTY(QStringList filesUrls READ filesUrls NOTIFY filesUrlsDone)
    Q_PROPERTY(QString fileUrl READ fileUrl NOTIFY fileUrlDone)


private:

#ifdef Q_OS_ANDROID
    AndroidFileDialog *dialog;
#endif
    QString _fileUrl;
    QString _title;
    QString _dir;
    QString _filter;
    QStringList _filesUrls;

private slots:
    void done(QString);

public:
    FileDialog(QObject *ptr = nullptr);
    ~FileDialog();
public slots:

    /**
     * @brief open a new files
     * result saved into filesUrl
     */
    void open();

    /**
     * @brief save - get default save file url
     * return file url
     */
    QString save(const QString &name);

    /**
     * @brief title
     * @return
     */
    QString title() const;

    /**
     * @brief dir
     * @return
     */
    QString dir() const;

    /**
     * @brief filter
     * @return
     */
    QString filter() const;

    /**
     * @brief filesUrls
     * @return list of files urls
     */
    QStringList filesUrls()const;

    /**
     * @brief fileUrl
     * @return file url
     */
    QString fileUrl()const;

    /**
     * @brief setTitle
     */
    void setTitle(const QString&);

    /**
     * @brief setDir
     */
    void setDir(const QString&);

    /**
     * @brief setfilter
     */
    void setFilter(const QString &);

signals:

    /**
     * @brief filesUrlsDone
     * Emited when files selected
     */
    void filesUrlsDone();

    /**
     * @brief filesUrlDone
     * Emited when files selected
     */
    void fileUrlDone();


};

#endif // FILEDIALOG_H
