#include "reportgenerator.h"
#include <QFile>
#include <QTextStream>

bool ReportGenerator::exportCSV(const QString& path, const QVector<Component>& items) {
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&f);
    out << "id,name,type,quantity,location,purchase_date\n";

    for (const Component& c : items) {
        out << c.id() << ","
            << "\"" << c.name() << "\","
            << "\"" << c.type() << "\","
            << c.quantity() << ","
            << "\"" << c.location() << "\","
            << c.purchaseDate().toString(Qt::ISODate) << "\n";
    }
    return true;
}

bool ReportGenerator::exportPDF(const QString& path, const QVector<Component>& items) {
    Q_UNUSED(path)
    Q_UNUSED(items)
    return false;
}
