#pragma once
#include <QSqlDatabase>
#include <QVector>
#include <QString>
#include "component.h"

class DatabaseManager {
public:
    explicit DatabaseManager(const QString& dbPath);
    ~DatabaseManager();

    bool open();
    void close();
    bool initSchema();

    int insertComponent(const Component& c);
    bool updateComponent(const Component& c);
    bool deleteComponent(int id);

    QVector<Component> getAllComponents();
    QVector<Component> searchComponents(const QString& term);

private:
    QSqlDatabase db_;
};
