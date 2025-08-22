#ifndef AUTOMOBIL_DATEN_H
#define AUTOMOBIL_DATEN_H

#include <QObject>
#include <qstring.h>
#include "sqlite3.h"

struct Automobil_Daten{
    QString Manufacturer;
    QString CarModell;
    QString CarModellVersion;
    QString fillingQuantity;
    QString OilTyp;
    QString OilVisk;
    QString Version;

};

class Automobil_DatenDB
{
public:
    Automobil_DatenDB(const QString &dbPath);
    ~Automobil_DatenDB();

    bool init();
    bool insertManufactuererData(const Automobil_Daten &am_d);
    bool importFromCSV(const QString &csvPath);
    QVector<Automobil_Daten> getAllAutomobilData();
private:
    sqlite3 *db;
};

#endif // AUTOMOBIL_DATEN_H
