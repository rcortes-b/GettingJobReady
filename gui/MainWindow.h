#pragma once
#include <QMainWindow>
#include "TaskManager.h"
#include "TaskTableModel.h"

class QTableView;
class QComboBox;
class QLabel;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(TaskManager& manager, QWidget* parent = nullptr);

private slots:
    void onAddTask();
    void onMarkDone();
    void onDeleteTask();
    void onFilterChanged(int index);
    void onSelectionChanged();

private:
    TaskManager&    manager_;
    TaskTableModel* model_;
    QTableView*     tableView_;
    QLabel*         statusLabel_;
    QPushButton*    doneBtn_;
    QPushButton*    deleteBtn_;

    int selectedTaskId() const;
    void updateStatusBar();
};
