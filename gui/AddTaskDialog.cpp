#include "AddTaskDialog.h"
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QDate>
#include <QPushButton>
#include <QLabel>

AddTaskDialog::AddTaskDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Nueva tarea");
    setMinimumWidth(380);

    titleEdit_ = new QLineEdit(this);
    titleEdit_->setPlaceholderText("Título de la tarea...");

    descEdit_ = new QTextEdit(this);
    descEdit_->setPlaceholderText("Descripción (opcional)");
    descEdit_->setMaximumHeight(80);

    priorityCombo_ = new QComboBox(this);
    priorityCombo_->addItems({"Medium", "Low", "High"});
    priorityCombo_->setCurrentIndex(0);

    dueDateEdit_ = new QDateEdit(this);
    dueDateEdit_->setCalendarPopup(true);
    dueDateEdit_->setDate(QDate::currentDate());
    dueDateEdit_->setSpecialValueText("Sin fecha");
    dueDateEdit_->setMinimumDate(QDate(2000, 1, 1));

    auto* form = new QFormLayout;
    form->addRow("Título *", titleEdit_);
    form->addRow("Descripción", descEdit_);
    form->addRow("Prioridad", priorityCombo_);
    form->addRow("Vencimiento", dueDateEdit_);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setText("Añadir");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    connect(buttons, &QDialogButtonBox::accepted, this, [this] {
        if (titleEdit_->text().trimmed().isEmpty()) {
            titleEdit_->setPlaceholderText("⚠ El título es obligatorio");
            titleEdit_->setStyleSheet("border: 1px solid red;");
            return;
        }
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);
}

QString AddTaskDialog::title() const {
    return titleEdit_->text().trimmed();
}

QString AddTaskDialog::description() const {
    return descEdit_->toPlainText().trimmed();
}

Priority AddTaskDialog::priority() const {
    return Task::priorityFromString(
        priorityCombo_->currentText().toLower().toStdString());
}

QString AddTaskDialog::dueDate() const {
    if (dueDateEdit_->date() == dueDateEdit_->minimumDate())
        return "";
    return dueDateEdit_->date().toString("yyyy-MM-dd");
}
