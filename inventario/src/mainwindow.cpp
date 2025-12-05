#include "mainwindow.h"
#include "reportgenerator.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>

MainWindow::MainWindow(InventoryManager* inv, QWidget* parent)
    : QMainWindow(parent),
      inv_(inv),
      table_(new QTableView(this)),
      model_(new QStandardItemModel(this)),
      nameEdit_(new QLineEdit(this)),
      typeEdit_(new QLineEdit(this)),
      quantitySpin_(new QSpinBox(this)),
      locationEdit_(new QLineEdit(this)),
      purchaseDateEdit_(new QDateEdit(this)),
      searchEdit_(new QLineEdit(this)),
      addButton_(new QPushButton("Agregar", this)),
      updateButton_(new QPushButton("Actualizar", this)),
      deleteButton_(new QPushButton("Eliminar", this)),
      exportCSVButton_(new QPushButton("Exportar CSV", this)) {

    setupUi();
    setupConnections();
    refreshTable(inv_->allComponents());
}

void MainWindow::setupUi() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    model_->setColumnCount(6);
    model_->setHeaderData(0, Qt::Horizontal, "ID");
    model_->setHeaderData(1, Qt::Horizontal, "Nombre");
    model_->setHeaderData(2, Qt::Horizontal, "Tipo");
    model_->setHeaderData(3, Qt::Horizontal, "Cantidad");
    model_->setHeaderData(4, Qt::Horizontal, "Ubicación");
    model_->setHeaderData(5, Qt::Horizontal, "Fecha compra");

    table_->setModel(model_);
    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);

    quantitySpin_->setMinimum(0);
    quantitySpin_->setMaximum(100000);
    purchaseDateEdit_->setCalendarPopup(true);
    purchaseDateEdit_->setDate(QDate::currentDate());

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Nombre:", nameEdit_);
    formLayout->addRow("Tipo:", typeEdit_);
    formLayout->addRow("Cantidad:", quantitySpin_);
    formLayout->addRow("Ubicación:", locationEdit_);
    formLayout->addRow("Fecha compra:", purchaseDateEdit_);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton_);
    buttonLayout->addWidget(updateButton_);
    buttonLayout->addWidget(deleteButton_);
    buttonLayout->addWidget(exportCSVButton_);

    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftLayout->addLayout(formLayout);
    leftLayout->addLayout(buttonLayout);

    QVBoxLayout* rightLayout = new QVBoxLayout;
    rightLayout->addWidget(new QLabel("Buscar:", this));
    rightLayout->addWidget(searchEdit_);
    rightLayout->addWidget(table_);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    central->setLayout(mainLayout);

    setWindowTitle("Gestor de Inventario");
    resize(900, 500);
}

void MainWindow::setupConnections() {
    connect(addButton_, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(updateButton_, &QPushButton::clicked, this, &MainWindow::onUpdateClicked);
    connect(deleteButton_, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(exportCSVButton_, &QPushButton::clicked, this, &MainWindow::onExportCSVClicked);
    connect(searchEdit_, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);

    connect(table_->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &MainWindow::onTableSelectionChanged);

    connect(inv_, &InventoryManager::dataChanged, [this]() {
        refreshTable(inv_->allComponents());
    });
}

void MainWindow::refreshTable(const QVector<Component>& items) {
    model_->removeRows(0, model_->rowCount());

    for (int i = 0; i < items.size(); ++i) {
        const Component& c = items[i];
        model_->insertRow(i);
        model_->setData(model_->index(i, 0), c.id());
        model_->setData(model_->index(i, 1), c.name());
        model_->setData(model_->index(i, 2), c.type());
        model_->setData(model_->index(i, 3), c.quantity());
        model_->setData(model_->index(i, 4), c.location());
        model_->setData(model_->index(i, 5), c.purchaseDate().toString(Qt::ISODate));
    }
}

void MainWindow::clearForm() {
    nameEdit_->clear();
    typeEdit_->clear();
    quantitySpin_->setValue(0);
    locationEdit_->clear();
    purchaseDateEdit_->setDate(QDate::currentDate());
}

Component MainWindow::componentFromForm(int existingId) const {
    return Component(
        existingId,
        nameEdit_->text(),
        typeEdit_->text(),
        quantitySpin_->value(),
        locationEdit_->text(),
        purchaseDateEdit_->date()
    );
}

void MainWindow::onAddClicked() {
    Component c = componentFromForm();
    int newId = inv_->addComponent(c);
    if (newId < 0) {
        QMessageBox::warning(this, "Error", "No se pudo agregar el componente.");
        return;
    }
    clearForm();
}

void MainWindow::onUpdateClicked() {
    QModelIndex current = table_->currentIndex();
    if (!current.isValid()) {
        QMessageBox::information(this, "Seleccionar", "Selecciona un componente para actualizar.");
        return;
    }
    int row = current.row();
    int id = model_->index(row, 0).data().toInt();
    Component c = componentFromForm(id);
    if (!inv_->updateComponent(c)) {
        QMessageBox::warning(this, "Error", "No se pudo actualizar el componente.");
    }
}

void MainWindow::onDeleteClicked() {
    QModelIndex current = table_->currentIndex();
    if (!current.isValid()) {
        QMessageBox::information(this, "Seleccionar", "Selecciona un componente para eliminar.");
        return;
    }
    int row = current.row();
    int id = model_->index(row, 0).data().toInt();

    if (QMessageBox::question(this, "Confirmar", "¿Eliminar este componente?") == QMessageBox::Yes) {
        if (!inv_->removeComponent(id)) {
            QMessageBox::warning(this, "Error", "No se pudo eliminar el componente.");
        }
    }
}

void MainWindow::onSearchTextChanged(const QString& text) {
    refreshTable(inv_->search(text));
}

void MainWindow::onExportCSVClicked() {
    QString path = QFileDialog::getSaveFileName(this, "Guardar CSV", "", "CSV (*.csv)");
    if (path.isEmpty()) return;

    QVector<Component> items = inv_->allComponents();
    if (!ReportGenerator::exportCSV(path, items)) {
        QMessageBox::warning(this, "Error", "No se pudo exportar el CSV.");
    }
}

void MainWindow::onTableSelectionChanged(const QModelIndex& current, const QModelIndex& previous) {
    Q_UNUSED(previous);
    if (!current.isValid()) {
        clearForm();
        return;
    }

    int row = current.row();
    nameEdit_->setText(model_->index(row, 1).data().toString());
    typeEdit_->setText(model_->index(row, 2).data().toString());
    quantitySpin_->setValue(model_->index(row, 3).data().toInt());
    locationEdit_->setText(model_->index(row, 4).data().toString());
    purchaseDateEdit_->setDate(QDate::fromString(model_->index(row, 5).data().toString(), Qt::ISODate));
}
