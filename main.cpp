#include <QCoreApplication>
#include <QDebug>
#include "Automobil_Daten.h"
#include <QDir>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "Working directory:" << QDir::currentPath();
    Automobile_DatenDB db("AutombilDatenDB.db");
    if(!db.init()) return 1;

    if(!db.importFromCSV("R1234yf.csv")){
        qDebug()<< "Error While loading CSV - Data";
        return 1;
    }


    if(!db.removeDuplicateData("AutomobilDatenDB")){
        qDebug() << "Duplicates couldn't be deleted!";
        return 1;
    }

    QVector<Automobile_Daten> data_cars = db.getAllAutomobilData();

    for(const Automobile_Daten &data_am : data_cars){
        qDebug() << data_am.Manufacturer << data_am.CarModell << data_am.CarModellVersion << data_am.fillingQuantity << data_am.OilTyp << data_am.OilVisk << data_am.Version;
    }

    return 0;
}
