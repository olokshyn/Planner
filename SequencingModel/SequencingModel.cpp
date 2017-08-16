#include "SequencingModel.h"

#include <algorithm>
#include <iterator>
#include <utility>

#include <QMimeData>
#include <QByteArray>
#include <QDataStream>
#include <QtDebug>

namespace
{

int findCol(const tree_algorithms::parallel_sequencing_t<std::string>& sequencing,
            const std::string& itemName)
{
    auto iter = std::find_if(sequencing.begin(), sequencing.end(),
                             [&itemName](const std::vector<std::string>& col) -> bool
    {
        return std::find(col.begin(), col.end(), itemName) != col.end();
    });
    if (iter != sequencing.end())
    {
        return static_cast<int>(iter - sequencing.begin());
    }
    return -1;
}

}

SequencingModel::SequencingModel(const tree_algorithms::parallel_sequencing_t<std::string>& lowerSequencing,
                                 const tree_algorithms::parallel_sequencing_t<std::string>& upperSequencing,
                                 QObject* parent)
    : QAbstractTableModel(parent),
      m_sequencing(lowerSequencing.size())
{
    if (lowerSequencing.size() != upperSequencing.size())
    {
        qFatal("Sequencing must be of the same size: %zu and %zu",
               lowerSequencing.size(), upperSequencing.size());
        return;
    }
    for (size_t col = 0; col != lowerSequencing.size(); ++col)
    {
        m_sequencing[col].reserve(lowerSequencing[col].size());
        for (size_t row = 0; row != lowerSequencing[col].size(); ++row)
        {
            int upperCol = findCol(upperSequencing, lowerSequencing[col][row]);
            if (upperCol == -1)
            {
                qFatal("Upper sequencing must contain exactly "
                       "the same elements as lower sequencing");
                return;
            }
            m_sequencing[col].emplace_back(QString::fromStdString(lowerSequencing[col][row]),
                                           static_cast<int>(col),
                                           upperCol);
        }
    }
}

Qt::ItemFlags SequencingModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

    if (index.isValid())
    {
        return defaultFlags
                | Qt::ItemIsDragEnabled
                | Qt::ItemIsDropEnabled
                | Qt::ItemIsEditable;
    }

    return defaultFlags | Qt::ItemIsDropEnabled;
}

int SequencingModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    if (m_sequencing.empty())
    {
        return 0;
    }
    std::vector<size_t> rowsCounts;
    rowsCounts.reserve(m_sequencing.size());
    std::transform(m_sequencing.begin(),
                   m_sequencing.end(),
                   std::back_inserter(rowsCounts),
                   [](const std::vector<Item>& col) -> size_t { return col.size(); });
    return static_cast<int>(*std::max_element(rowsCounts.begin(),
                                              rowsCounts.end()));
}

int SequencingModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(m_sequencing.size());
}

QVariant SequencingModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.column() < 0 || index.column() >= static_cast<int>(m_sequencing.size()))
    {
        return QVariant();
    }

    if (index.row() < 0 || index.row() >= static_cast<int>(m_sequencing[index.column()].size()))
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return QVariant(m_sequencing[index.column()][index.row()].name);
    }
    else
    {
        return QVariant();
    }
}

bool SequencingModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        if (index.column() >= 0
            && index.column() < static_cast<int>(m_sequencing.size()))
        {
            auto itemPos = findItemPos(value.toString());
            if (itemPos.first == -1)
            {
                qCritical() << "Cannot add new item:" << value.toString();
                return false;
            }

            if (index.column() < m_sequencing[itemPos.first][itemPos.second].leftBorder
                || index.column() > m_sequencing[itemPos.first][itemPos.second].rightBorder)
            {
                qDebug() << "Cannot move item"
                         << m_sequencing[itemPos.first][itemPos.second].name
                         << "to column" << index.column()
                         << "due to item`s borders:"
                         << m_sequencing[itemPos.first][itemPos.second].leftBorder << "-"
                         << m_sequencing[itemPos.first][itemPos.second].rightBorder;
                return false;
            }

            if (index.row() >= 0
                && index.row() < static_cast<int>(m_sequencing[index.column()].size()))
            {
                if (itemPos.first < m_sequencing[index.column()][index.row()].leftBorder
                    || itemPos.first > m_sequencing[index.column()][index.row()].rightBorder)
                {
                    qDebug() << "Cannot move item"
                             << m_sequencing[index.column()][index.row()].name
                             << "to column" << itemPos.first
                             << "due to item`s borders:"
                             << m_sequencing[index.column()][index.row()].leftBorder << "-"
                             << m_sequencing[index.column()][index.row()].rightBorder;
                    return false;
                }

                using std::swap;
                swap(m_sequencing[index.column()][index.row()],
                     m_sequencing[itemPos.first][itemPos.second]);

                QModelIndex itemIndex = createIndex(itemPos.second, itemPos.first);
                emit dataChanged(itemIndex, itemIndex);
                emit dataChanged(index, index);
            }
            else
            {
                const Item& item = m_sequencing[itemPos.first][itemPos.second];
                m_sequencing[index.column()].emplace_back(item.name, item.leftBorder, item.rightBorder);
                m_sequencing[itemPos.first].erase(m_sequencing[itemPos.first].begin() + itemPos.second);

                QModelIndex itemIndex = createIndex(
                                            static_cast<int>(m_sequencing[index.column()].size()) - 1,
                                            index.column());
                emit dataChanged(itemIndex, itemIndex);
                emit dataChanged(createIndex(0, itemPos.first),
                                 createIndex(
                                     static_cast<int>(m_sequencing[itemPos.first].size()) - 1,
                                     itemPos.first));
            }
            return true;
        }
    }
    return false;
}

bool SequencingModel::insertRows(int row, int count, const QModelIndex& parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    qCritical() << "Cannot add new rows";
    return false;
}

bool SequencingModel::removeRows(int row, int count, const QModelIndex& parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    qCritical() << "Cannot remove rows";
    return false;
}

bool SequencingModel::insertColumns(int column, int count, const QModelIndex& parent)
{
    Q_UNUSED(column);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    qCritical() << "Cannot add columns";
    return false;
}

bool SequencingModel::removeColumns(int column, int count, const QModelIndex& parent)
{
    Q_UNUSED(column);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    qCritical() << "Cannot remove columns";
    return false;
}

Qt::DropActions SequencingModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList SequencingModel::mimeTypes() const
{
    QStringList types;
    types << "application/planner.item.name";
    return types;
}

QMimeData* SequencingModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    if (indexes.size() != 1)
    {
        qWarning() << "Unable to export more than one item at a time";
        if (indexes.empty())
        {
            qCritical("Cannot create mime data from an empty list of indexes");
            return nullptr;
        }
    }
    QModelIndex index = indexes.first();
    if (index.isValid()
        && index.column() >= 0 && index.column() < static_cast<int>(m_sequencing.size())
        && index.row() >= 0 && index.row() < static_cast<int>(m_sequencing[index.column()].size()))
    {
        stream << m_sequencing[index.column()][index.row()].name;
    }
    mimeData->setData("application/planner.item.name", encodedData);
    return mimeData;
}

bool SequencingModel::dropMimeData(const QMimeData* data, Qt::DropAction action,
                                   int row, int column, const QModelIndex& parent)
{
    if (action == Qt::IgnoreAction)
    {
        return true;
    }

    if (!data->hasFormat("application/planner.item.name"))
    {
        return false;
    }

    if (column >= static_cast<int>(m_sequencing.size()))
    {
        qWarning() << "Cannot extend the sequencing";
        return false;
    }

    QByteArray encodedData = data->data("application/planner.item.name");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    QString itemName;
    stream >> itemName;

    if (column != -1)
    {
        setData(createIndex(row, column), QVariant(itemName));
    }
    else if (parent.isValid())
    {
        setData(parent, QVariant(itemName));
    }
    else
    {
        qWarning() << "Invalid drop position of item" << itemName;
        return false;
    }
    return true;
}

SequencingModel::Item::Item()
    : leftBorder(0),
      rightBorder(0)
{

}

SequencingModel::Item::Item(const QString& name, int leftBorder, int rightBorder)
    : name(name),
      leftBorder(leftBorder),
      rightBorder(rightBorder)
{

}

std::pair<int, int> SequencingModel::findItemPos(const QString& itemName) const
{
    for (size_t col = 0; col != m_sequencing.size(); ++col)
    {
        auto iter = std::find_if(m_sequencing[col].begin(),
                                 m_sequencing[col].end(),
                                 [&itemName](const Item& item) -> bool { return item.name == itemName; });
        if (iter != m_sequencing[col].end())
        {
            return std::make_pair(static_cast<int>(col),
                                  static_cast<int>(iter - m_sequencing[col].begin()));
        }
    }
    return std::make_pair(-1, -1);
}
