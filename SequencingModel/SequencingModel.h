#ifndef SEQUENCINGMODEL_H
#define SEQUENCINGMODEL_H

#include <QAbstractTableModel>

#include "TreeAlgorithms.h"

class SequencingModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    SequencingModel(const tree_algorithms::parallel_sequencing_t<std::string>& lowerSequencing,
                    const tree_algorithms::parallel_sequencing_t<std::string>& upperSequencing,
                    QObject* parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;

    Qt::DropActions supportedDropActions() const override;

    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList& indexes) const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action,
                      int row, int column, const QModelIndex& parent) override;

private:
    struct Item
    {
        Item();
        Item(const QString& name, int leftBorder, int rightBorder);

        QString name;
        int leftBorder;
        int rightBorder;
    };

private:
    std::pair<int, int> findItemPos(const QString& itemName) const;

private:
    tree_algorithms::parallel_sequencing_t<Item> m_sequencing;
};

#endif // SEQUENCINGMODEL_H
