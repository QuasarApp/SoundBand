#ifndef ABSTRACTPACKAGE_H
#define ABSTRACTPACKAGE_H
#include <QByteArray>

class AbstractPackage
{
public:
    AbstractPackage(){}

    /**
     * @brief isValid
     * @return true if package is valid
     */
     virtual bool isValid() = 0;

    /**
     * @brief clear all date of package
     */
    virtual void clear() = 0;

    /**
     * @brief parseTo parse this package to byte array
     * @return byte array
     */
    virtual QByteArray parseTo() = 0;

    /**
     * @brief parseFrom create a package from bytes
     * @param array of bytes
     * @return true if package valid
     */
    virtual bool parseFrom(QByteArray& array) = 0;

    virtual ~AbstractPackage(){}
};

#endif // ABSTRACTPACKAGE_H
