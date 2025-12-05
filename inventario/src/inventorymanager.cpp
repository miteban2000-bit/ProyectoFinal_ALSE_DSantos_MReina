#include "inventorymanager.h"

InventoryManager::InventoryManager(DatabaseManager* db,
                                   int lowStockThreshold,
                                   QObject* parent)
    : QObject(parent),
      db_(db),
      lowStockThreshold_(lowStockThreshold) {}

QVector<Component> InventoryManager::allComponents() {
    return db_->getAllComponents();
}

QVector<Component> InventoryManager::search(const QString& term) {
    if (term.trimmed().isEmpty()) {
        return allComponents();
    }
    return db_->searchComponents(term);
}

int InventoryManager::addComponent(const Component& c) {
    int id = db_->insertComponent(c);
    emit dataChanged();
    return id;
}

bool InventoryManager::updateComponent(const Component& c) {
    bool ok = db_->updateComponent(c);
    if (ok) emit dataChanged();
    return ok;
}

bool InventoryManager::removeComponent(int id) {
    bool ok = db_->deleteComponent(id);
    if (ok) emit dataChanged();
    return ok;
}
