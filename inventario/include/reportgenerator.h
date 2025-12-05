#pragma once
#include <QString>
#include <QVector>
#include "component.h"

class ReportGenerator {
public:
    static bool exportCSV(const QString& path, const QVector<Component>& items);
    static bool exportPDF(const QString& path, const QVector<Component>& items);
};

