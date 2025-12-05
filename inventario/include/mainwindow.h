#pragma once
#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDateEdit>
#include <QPushButton>
#include <QModelIndex>
#include "inventorymanager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT   // ✅ AGREGA ESTO

public:
    explicit MainWindow(InventoryManager* inv, QWidget* parent = nullptr);

private slots:
    void onAddClicked();
    void onUpdateClicked();
    void onDeleteClicked();
    void onSearchTextChanged(const QString& text);
    void onExportCSVClicked();
    void onTableSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

private:
    void setupUi();
    void setupConnections();
    void refreshTable(const QVector<Component>& items);
    void clearForm();
    Component componentFromForm(int existingId = -1) const;

    InventoryManager* inv_;
    QTableView* table_;
    QStandardItemModel* model_;

    QLineEdit* nameEdit_;
    QLineEdit* typeEdit_;
    QSpinBox* quantitySpin_;
    QLineEdit* locationEdit_;
    QDateEdit* purchaseDateEdit_;
    QLineEdit* searchEdit_;

    QPushButton* addButton_;
    QPushButton* updateButton_;
    QPushButton* deleteButton_;
    QPushButton* exportCSVButton_;
};
