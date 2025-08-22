#include "automobil_daten.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "sqlite3.h"

Automobil_DatenDB::Automobil_DatenDB(const QString &dbPath) {
    if(sqlite3_open(dbPath.toUtf8().constData(), &db) != SQLITE_OK){
        qDebug() << "Error while Loading Database: " <<sqlite3_errmsg(db);
        db = nullptr;
    }
}

Automobil_DatenDB::~Automobil_DatenDB(){
    if(db)sqlite3_close(db);
}

bool Automobil_DatenDB::init(){
    const char *sqlcreate = R"(CREATE TABLE IF NOT EXISTS person (
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

bool Automobil_DatenDB::insertManufactuererData(const Automobil_Daten &am_d){
    const char *sql = "Insert Manufacturer: ";
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

bool Automobil_DatenDB::importFromCSV(const QString &csvPath){
    QFile file(csvPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream in(&file);
    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if(parts.size() !=26) continue;

        Automobil_Daten data_am;
        data_am.Manufacturer = parts[0].trimmed();
        data_am.CarModell =parts[1].trimmed();
        data_am.CarModellVersion =parts[2].trimmed();
        data_am.fillingQuantity = parts[8].trimmed();
        data_am.OilTyp = parts[10].trimmed();
        data_am.OilVisk = parts[11].trimmed();
        data_am.Version=parts[25].trimmed();
        insertManufactuererData(data_am);

    }
    return true;

}

QVector<Automobil_Daten> Automobil_DatenDB::getAllAutomobilData(){

    QVector<Automobil_Daten> list;
    const char *sql = "SELECT Manufacturer , ....";
    sqlite3_stmt *stmt;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
        return list;
    }

    while(sqlite3_step(stmt) == SQLITE_ROW){
        Automobil_Daten data_am;
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
