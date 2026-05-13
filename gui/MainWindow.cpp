#include "MainWindow.h"
#include "AddTaskDialog.h"
#include <QTableView>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QToolBar>
#include <QHeaderView>
#include <QStatusBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QAction>
#include <QItemSelectionModel>

MainWindow::MainWindow(TaskManager& manager, QWidget* parent)
    : QMainWindow(parent), manager_(manager) {
    setWindowTitle("Gestor de Tareas");
    setMinimumSize(750, 450);
    resize(900, 550);

    // --- Toolbar ---
    auto* toolbar = addToolBar("Acciones");
    toolbar->setMovable(false);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    auto* addBtn = new QPushButton("➕  Añadir", this);
    addBtn->setFixedHeight(30);
    toolbar->addWidget(addBtn);
    toolbar->addSeparator();

    doneBtn_ = new QPushButton("✔  Hecha", this);
    doneBtn_->setFixedHeight(30);
    doneBtn_->setEnabled(false);
    toolbar->addWidget(doneBtn_);

    deleteBtn_ = new QPushButton("🗑  Eliminar", this);
    deleteBtn_->setFixedHeight(30);
    deleteBtn_->setEnabled(false);
    toolbar->addWidget(deleteBtn_);

    toolbar->addSeparator();
    toolbar->addWidget(new QLabel("  Filtrar por prioridad: "));

    auto* filterCombo = new QComboBox(this);
    filterCombo->addItems({"Todas", "Low", "Medium", "High"});
    filterCombo->setFixedWidth(120);
    toolbar->addWidget(filterCombo);

    // --- Table ---
    model_ = new TaskTableModel(manager_, this);
    tableView_ = new QTableView(this);
    tableView_->setModel(model_);
    tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView_->setAlternatingRowColors(true);
    tableView_->setSortingEnabled(false);
    tableView_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableView_->horizontalHeader()->setDefaultSectionSize(110);
    tableView_->verticalHeader()->setVisible(false);
    tableView_->setColumnWidth(0, 45);

    setCentralWidget(tableView_);

    // --- Status bar ---
    statusLabel_ = new QLabel(this);
    statusBar()->addWidget(statusLabel_);
    updateStatusBar();

    // --- Connections ---
    connect(addBtn,    &QPushButton::clicked, this, &MainWindow::onAddTask);
    connect(doneBtn_,  &QPushButton::clicked, this, &MainWindow::onMarkDone);
    connect(deleteBtn_,&QPushButton::clicked, this, &MainWindow::onDeleteTask);
    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFilterChanged);
    connect(tableView_->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectionChanged);
}

int MainWindow::selectedTaskId() const {
    auto rows = tableView_->selectionModel()->selectedRows();
    if (rows.isEmpty()) return -1;
    return model_->data(model_->index(rows.first().row(), 0)).toInt();
}

void MainWindow::updateStatusBar() {
    int total = static_cast<int>(manager_.listAll().size());
    auto done = manager_.filter(Status::DONE, std::nullopt);
    statusLabel_->setText(
        QString("Total: %1  |  Completadas: %2  |  Pendientes: %3")
            .arg(total).arg(done.size()).arg(total - (int)done.size()));
}

void MainWindow::onAddTask() {
    AddTaskDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) return;

    manager_.add(
        dlg.title().toStdString(),
        dlg.description().toStdString(),
        dlg.priority(),
        dlg.dueDate().toStdString()
    );
    model_->refresh();
    updateStatusBar();
}

void MainWindow::onMarkDone() {
    int id = selectedTaskId();
    if (id < 0) return;

    auto task = manager_.get(id);
    if (task && task->status == Status::DONE) {
        manager_.updateStatus(id, Status::TODO);
    } else {
        manager_.updateStatus(id, Status::DONE);
    }
    model_->refresh();
    updateStatusBar();
}

void MainWindow::onDeleteTask() {
    int id = selectedTaskId();
    if (id < 0) return;

    auto task = manager_.get(id);
    QString title = task ? QString::fromStdString(task->title) : QString("#%1").arg(id);

    auto reply = QMessageBox::question(
        this, "Eliminar tarea",
        QString("¿Eliminar \"%1\"?").arg(title),
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    manager_.remove(id);
    model_->refresh();
    updateStatusBar();
}

void MainWindow::onFilterChanged(int index) {
    std::optional<Priority> filter;
    if (index == 1) filter = Priority::LOW;
    if (index == 2) filter = Priority::MEDIUM;
    if (index == 3) filter = Priority::HIGH;
    model_->applyFilter(filter);
}

void MainWindow::onSelectionChanged() {
    bool sel = selectedTaskId() >= 0;
    doneBtn_->setEnabled(sel);
    deleteBtn_->setEnabled(sel);

    if (sel) {
        auto task = manager_.get(selectedTaskId());
        if (task)
            doneBtn_->setText(task->status == Status::DONE
                              ? "↩  Reabrir" : "✔  Hecha");
    }
}
