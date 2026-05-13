#include "TaskTableModel.h"
#include <QColor>
#include <QFont>

static const QStringList kHeaders = {"ID", "Título", "Estado", "Prioridad", "Vencimiento"};

TaskTableModel::TaskTableModel(TaskManager& manager, QObject* parent)
    : QAbstractTableModel(parent), manager_(manager) {
    refresh();
}

void TaskTableModel::refresh() {
    beginResetModel();
    visible_ = manager_.filter(std::nullopt, currentFilter_);
    endResetModel();
}

void TaskTableModel::applyFilter(std::optional<Priority> priority) {
    currentFilter_ = priority;
    refresh();
}

int TaskTableModel::rowCount(const QModelIndex&) const {
    return static_cast<int>(visible_.size());
}

int TaskTableModel::columnCount(const QModelIndex&) const {
    return kHeaders.size();
}

QVariant TaskTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return kHeaders.value(section);
    return {};
}

QVariant TaskTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(visible_.size()))
        return {};

    const Task& t = visible_[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return t.id;
            case 1: return QString::fromStdString(t.title);
            case 2: return QString::fromStdString(Task::statusToString(t.status));
            case 3: return QString::fromStdString(Task::priorityToString(t.priority));
            case 4: return t.due_date.empty()
                         ? QString("—")
                         : QString::fromStdString(t.due_date);
        }
    }

    if (role == Qt::ForegroundRole) {
        if (t.status == Status::DONE)
            return QColor(Qt::gray);
        if (t.priority == Priority::HIGH)
            return QColor(Qt::red);
    }

    if (role == Qt::FontRole && t.status == Status::DONE) {
        QFont f;
        f.setStrikeOut(true);
        return f;
    }

    return {};
}
