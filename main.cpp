#include <QCoreApplication>
#include <QDebug>
#include "automobil_daten.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Automobil_DatenDB db("AutombilDatenDB.db");

    if(!db.init()) return 1;

    db.importFromCSV("1234fy.csv");

    QVector<Automobil_Daten> data_cars = db.getAllAutomobilData();

    for(const Automobil_Daten &data_am : data_cars){
        qDebug() << data_am.Manufacturer << data_am.CarModell << data_am.CarModellVersion << data_am.fillingQuantity << data_am.OilTyp << data_am.OilVisk << data_am.Version;
    }

    return 0;
}
