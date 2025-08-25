#ifndef Automobile_Daten_H
#define Automobile_Daten_H

#include <QObject>
#include <qstring.h>
#include "sqlite3.h"

struct Automobile_Daten{
    QString Manufacturer;
    QString CarModell;
    QString CarModellVersion;
    QString fillingQuantity;
    QString OilTyp;
    QString OilVisk;
    QString Version;

};

class Automobile_DatenDB
{
public:
    Automobile_DatenDB(const QString &dbPath);
    ~Automobile_DatenDB();

    bool init();
    bool insertManufactuererData(const Automobile_Daten &am_d);
    bool importFromCSV(const QString &csvPath);
    QVector<Automobile_Daten> getAllAutomobilData();
    bool removeDuplicateData(const QString &db_path);


private:
    sqlite3 *db;
};

#endif // Automobile_Daten_H
