#ifndef FILEDIALOG_H
#define FILEDIALOG_H
#include <QFileDialog>
#include "androidfiledialog.h"

/**
 * @brief The FileDialog class general crossplatform dialog window
 */
class FileDialog: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList filesUrl READ filesUrl NOTIFY loadComlete)
    Q_PROPERTY(QString saveUrl READ saveUrl NOTIFY saveComplete)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString dir READ dir WRITE setDir)
    Q_PROPERTY(QStringList filter READ filter WRITE setfilter)

private:

#ifdef Q_OS_ANDROID
    AndroidFileDialog *dialog;
#else
    QFileDialog *dialog;
#endif

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
     * @brief save get save file url
     */
    void save();

    /**
     * @brief filesUrl
     * @return
     */
    QStringList filesUrl();

    /**
     * @brief saveUrl
     * @return
     */
    QString saveUrl();

    /**
     * @brief title
     * @return
     */
    QString title();

    /**
     * @brief dir
     * @return
     */
    QString dir();

    /**
     * @brief filter
     * @return
     */
    QStringList filter();

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
    void setfilter(const QStringList&);

signals:

    /**
     * @brief loadComlete emited then load is complete
     */
    void loadComlete();

    /**
     * @brief saveComplete emited then load is complete
     */
    void saveComplete();

};

#endif // FILEDIALOG_H
