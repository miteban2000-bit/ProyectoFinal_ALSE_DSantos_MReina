#include <QApplication>
#include "mainwindow.h"
#include "databasemanager.h"
#include "inventorymanager.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    DatabaseManager db("inventory.sqlite");
    if (!db.open()) return 1;
    if (!db.initSchema()) return 1;

    InventoryManager inv(&db, 1);
    MainWindow w(&inv);
    w.show();

    return app.exec();
}
