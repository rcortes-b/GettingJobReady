#pragma once
#include <QAbstractTableModel>
#include "TaskManager.h"

class TaskTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit TaskTableModel(TaskManager& manager, QObject* parent = nullptr);

    int      rowCount(const QModelIndex& parent = {}) const override;
    int      columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    void applyFilter(std::optional<Priority> priority);
    void refresh();

private:
    TaskManager&      manager_;
    std::vector<Task> visible_;
    std::optional<Priority> currentFilter_;
};
