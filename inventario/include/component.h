#pragma once
#include <QString>
#include <QDate>

class Component {
public:
    Component() = default;

    Component(int id,
              const QString& name,
              const QString& type,
              int quantity,
              const QString& location,
              const QDate& purchaseDate)
        : id_(id),
          name_(name),
          type_(type),
          quantity_(quantity),
          location_(location),
          purchaseDate_(purchaseDate) {}

    int id() const { return id_; }
    QString name() const { return name_; }
    QString type() const { return type_; }
    int quantity() const { return quantity_; }
    QString location() const { return location_; }
    QDate purchaseDate() const { return purchaseDate_; }

    void setId(int id) { id_ = id; }
    void setName(const QString& name) { name_ = name; }
    void setType(const QString& type) { type_ = type; }
    void setQuantity(int quantity) { quantity_ = quantity; }
    void setLocation(const QString& location) { location_ = location; }
    void setPurchaseDate(const QDate& date) { purchaseDate_ = date; }

    bool isLowStock(int threshold) const { return quantity_ <= threshold; }

private:
    int id_{-1};
    QString name_;
    QString type_;
    int quantity_{0};
    QString location_;
    QDate purchaseDate_;
};
