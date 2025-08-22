#include "sqlite3.h"
#include <QDebug>
#include <isostream>

bool initDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("CarManufacturer");

    if(!db.open()){
        qDebug() << "Failed loading the Database: " << sb.lastError().text;
        return false;
    }
}


