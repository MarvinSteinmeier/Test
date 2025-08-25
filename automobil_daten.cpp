#include "Automobil_Daten.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "sqlite3.h"

Automobile_DatenDB::Automobile_DatenDB(const QString &dbPath) {
    if(sqlite3_open(dbPath.toUtf8().constData(), &db) != SQLITE_OK){
        qDebug() << "Error while Loading Database: " <<sqlite3_errmsg(db);
        db = nullptr;
    }
}

Automobile_DatenDB::~Automobile_DatenDB(){
    if(db)sqlite3_close(db);
}

bool Automobile_DatenDB::init(){
    const char *sqlcreate = R"(CREATE TABLE IF NOT EXISTS AutomobilDatenDB (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            Manufacturer TEXT,
            CarModell TEXT,
            CarModellVersion TEXT,
            FillingQuantitity TEXT,
            OilTyp TEXT,
            OilVisk TEXT,
            Version TEXT
        );
    )";

    char *errMSG = nullptr;
    int rc = sqlite3_exec(db, sqlcreate, nullptr, nullptr, &errMSG);

    if(rc != SQLITE_OK){
        qDebug() <<"Error while Loading Database: " <<errMSG;
        sqlite3_free(errMSG);
        return false;
    }
    return true;
}

bool Automobile_DatenDB::insertManufactuererData(const Automobile_Daten &am_d){
    const char *sql = R"(
      INSERT INTO AutomobilDatenDB (
          Manufacturer, CarModell, CarModellVersion,
         FillingQuantitity, OilTyp, OilVisk, Version
        ) VALUES (?, ?, ?, ?, ?, ?, ?);
    )";
    sqlite3_stmt *stmt;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
        return false;
    }
    sqlite3_bind_text(stmt, 1, am_d.Manufacturer.toUtf8().constData(), -1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, am_d.CarModell.toUtf8().constData(), -1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, am_d.CarModellVersion.toUtf8().constData(), -1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, am_d.fillingQuantity.toUtf8().constData(), -1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, am_d.OilTyp.toUtf8().constData(), -1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, am_d.OilVisk.toUtf8().constData(), -1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, am_d.Version.toUtf8().constData(), -1,SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;

}

bool Automobile_DatenDB::importFromCSV(const QString &csvPath){
    QFile file(csvPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Fehler beim Öffnen der Datei!";
        return false;
    }
    QTextStream in(&file);
    while(!in.atEnd()){
        QString line = in.readLine();

        QStringList parts = line.split(';');
        if(parts.size() !=25) continue;
        Automobile_Daten data_am;
        data_am.Manufacturer = parts[0].trimmed();
        data_am.CarModell =parts[1].trimmed();
        data_am.CarModellVersion =parts[2].trimmed();
        data_am.fillingQuantity = parts[8].trimmed();
        data_am.OilTyp = parts[9].trimmed();
        data_am.OilVisk = parts[11].trimmed();
        data_am.Version=parts[24].trimmed();
        insertManufactuererData(data_am);

    }
    return true;

}

QVector<Automobile_Daten> Automobile_DatenDB::getAllAutomobilData(){

    QVector<Automobile_Daten> list;
    const char *sql = R"(
    SELECT Manufacturer, CarModell, CarModellVersion,
           FillingQuantitity, OilTyp, OilVisk, Version
            FROM AutomobilDatenDB;
    )";
    sqlite3_stmt *stmt;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
        return list;
    }

    while(sqlite3_step(stmt) == SQLITE_ROW){
        Automobile_Daten data_am;
        data_am.Manufacturer = reinterpret_cast <const char*>(sqlite3_column_text(stmt,0));
        data_am.CarModell = reinterpret_cast <const char*>(sqlite3_column_text(stmt,1));
        data_am.CarModellVersion = reinterpret_cast <const char*>(sqlite3_column_text(stmt,2));
        data_am.fillingQuantity = reinterpret_cast <const char*>(sqlite3_column_text(stmt,3));
        data_am.OilTyp = reinterpret_cast <const char*>(sqlite3_column_text(stmt,4));
        data_am.OilVisk = reinterpret_cast <const char*>(sqlite3_column_text(stmt,5));
        data_am.Version = reinterpret_cast <const char*>(sqlite3_column_text(stmt,6));

        list.append(data_am);
    }

    sqlite3_finalize(stmt);
    return list;
}

bool Automobile_DatenDB::removeDuplicateData(const QString &dbPath){
    if(sqlite3_open(dbPath.toUtf8().constData(), &db) != SQLITE_OK){
        qDebug() << "Error while Loading Database: " <<sqlite3_errmsg(db);
        db = nullptr;
    }

    const char *sql = R"(DELTE FROM AutomobilDatenDB WHERE rowid NOT IN(
            SELECT MIN(rowid) FROM AutomobilDatenDB
            GROUP BY Manufactuer, CarModell
        );
    )";

    char *errMSG = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr,nullptr, &errMSG);

    if(rc!=SQLITE_OK){
        qDebug() <<"Error while Loading Database: " <<errMSG;
        sqlite3_free(errMSG);
        return false;
    }
    qDebug() << "Duplicates were deleted!";
    return true;


}
