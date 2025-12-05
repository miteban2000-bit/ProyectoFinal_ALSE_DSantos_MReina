#pragma once
#include <QObject>
#include <QVector>
#include "component.h"
#include "databasemanager.h"

class InventoryManager : public QObject {
    Q_OBJECT   // ✅ AGREGA ESTO

public:
    explicit InventoryManager(DatabaseManager* db,
                              int lowStockThreshold = 1,
                              QObject* parent = nullptr);

    QVector<Component> allComponents();
    QVector<Component> search(const QString& term);

    int addComponent(const Component& c);
    bool updateComponent(const Component& c);
    bool removeComponent(int id);

    bool isLowStock(const Component& c) const { return c.isLowStock(lowStockThreshold_); }
    void setLowStockThreshold(int t) { lowStockThreshold_ = t; }
    int lowStockThreshold() const { return lowStockThreshold_; }

signals:
    void dataChanged();   // ✅ ESTA SEÑAL NECESITA Q_OBJECT

private:
    DatabaseManager* db_;
    int lowStockThreshold_;
};
