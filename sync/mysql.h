#ifndef MYSQL_H
#define MYSQL_H
#include <QString>


class QSqlDatabase;
class QSqlQuery;

namespace syncLib {


class MySql
{
public:
    MySql();
    static bool exec(QSqlQuery *q, const QString& sqlFile);
    ~MySql();
};
}
#endif // MYSQL_H
