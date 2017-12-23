#include "mysql.h"
#include <QSqlQuery>
#include <QtSql>

namespace syncLib{

MySql::MySql()
{

}

bool MySql::exec(QSqlQuery *sq,const QString& sqlFile){
    QFile f(sqlFile);
    bool result=true;
    if(f.open(QIODevice::ReadOnly)){
        QString temp,delimiter=";";
        QTextStream stream(&f);
        stream.setCodec("UTF8");
        while(!stream.atEnd()){
            temp+=stream.readLine();
            if(temp.lastIndexOf("delimiter",-1,Qt::CaseInsensitive)>-1){
                temp.remove("delimiter",Qt::CaseInsensitive);
                int last=temp.indexOf(QRegularExpression("[^ \f\n\r\t\v]"))+1;
                int begin=temp.lastIndexOf(QRegularExpression("[^ \f\n\r\t\v]"));
                delimiter=temp.mid(begin,last-begin);
                temp="";
            }else{
                if(temp.lastIndexOf(delimiter)>-1){
                    temp.remove(delimiter);
                    (result=result&&sq->exec(temp));
                    temp="";
                }
            }
        }
        return result;
    }
    return false;

}
}

