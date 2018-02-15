#include "filedialog.h"

FileDialog::FileDialog(QObject *ptr):
    QObject(ptr)
{
#ifdef Q_OS_ANDROID
#else
    QFileDialog *dialog = new QFileDialog(this, );
#endif
}
