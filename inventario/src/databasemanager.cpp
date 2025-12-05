#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

DatabaseManager::DatabaseManager(const QString& dbPath) {
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbPath);
}

DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::open() {
    if (!db_.open()) {
        qWarning() << "Failed to open database:" << db_.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::close() {
    if (db_.isOpen()) {
        db_.close();
    }
}

bool DatabaseManager::initSchema() {
    QSqlQuery q;
    const char* createTableSql =
        "CREATE TABLE IF NOT EXISTS components ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "type TEXT NOT NULL,"
        "quantity INTEGER NOT NULL,"
        "location TEXT,"
        "purchase_date TEXT"
        ");";
    if (!q.exec(createTableSql)) {
        qWarning() << "Failed to create table:" << q.lastError().text();
        return false;
    }
    return true;
}

int DatabaseManager::insertComponent(const Component& c) {
    QSqlQuery q;
    q.prepare("INSERT INTO components "
              "(name, type, quantity, location, purchase_date) "
              "VALUES (?, ?, ?, ?, ?);");
    q.addBindValue(c.name());
    q.addBindValue(c.type());
    q.addBindValue(c.quantity());
    q.addBindValue(c.location());
    q.addBindValue(c.purchaseDate().toString(Qt::ISODate));

    if (!q.exec()) {
        qWarning() << "Failed to insert component:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

bool DatabaseManager::updateComponent(const Component& c) {
    if (c.id() < 0) return false;

    QSqlQuery q;
    q.prepare("UPDATE components SET "
              "name = ?, type = ?, quantity = ?, location = ?, purchase_date = ? "
              "WHERE id = ?;");
    q.addBindValue(c.name());
    q.addBindValue(c.type());
    q.addBindValue(c.quantity());
    q.addBindValue(c.location());
    q.addBindValue(c.purchaseDate().toString(Qt::ISODate));
    q.addBindValue(c.id());

    if (!q.exec()) {
        qWarning() << "Failed to update component:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool DatabaseManager::deleteComponent(int id) {
    if (id < 0) return false;

    QSqlQuery q;
    q.prepare("DELETE FROM components WHERE id = ?;");
    q.addBindValue(id);

    if (!q.exec()) {
        qWarning() << "Failed to delete component:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}

QVector<Component> DatabaseManager::getAllComponents() {
    QVector<Component> result;
    QSqlQuery q("SELECT id, name, type, quantity, location, purchase_date FROM components;");

    while (q.next()) {
        int id = q.value(0).toInt();
        QString name = q.value(1).toString();
        QString type = q.value(2).toString();
        int quantity = q.value(3).toInt();
        QString location = q.value(4).toString();
        QDate purchaseDate = QDate::fromString(q.value(5).toString(), Qt::ISODate);

        result.append(Component(id, name, type, quantity, location, purchaseDate));
    }
    return result;
}

QVector<Component> DatabaseManager::searchComponents(const QString& term) {
    QVector<Component> result;
    QSqlQuery q;
    q.prepare("SELECT id, name, type, quantity, location, purchase_date "
              "FROM components "
              "WHERE name LIKE ? OR type LIKE ? OR location LIKE ?;");
    QString likeTerm = "%" + term + "%";
    q.addBindValue(likeTerm);
    q.addBindValue(likeTerm);
    q.addBindValue(likeTerm);

    if (!q.exec()) {
        qWarning() << "Failed to search components:" << q.lastError().text();
        return result;
    }

    while (q.next()) {
        int id = q.value(0).toInt();
        QString name = q.value(1).toString();
        QString type = q.value(2).toString();
        int quantity = q.value(3).toInt();
        QString location = q.value(4).toString();
        QDate purchaseDate = QDate::fromString(q.value(5).toString(), Qt::ISODate);

        result.append(Component(id, name, type, quantity, location, purchaseDate));
    }
    return result;
}
