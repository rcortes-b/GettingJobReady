#pragma once
#include <QDialog>
#include <QString>
#include "Task.h"

class QLineEdit;
class QComboBox;
class QDateEdit;
class QTextEdit;

class AddTaskDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddTaskDialog(QWidget* parent = nullptr);

    QString  title()       const;
    QString  description() const;
    Priority priority()    const;
    QString  dueDate()     const;

private:
    QLineEdit* titleEdit_;
    QTextEdit* descEdit_;
    QComboBox* priorityCombo_;
    QDateEdit* dueDateEdit_;
};
