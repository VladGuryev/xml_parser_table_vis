#include "model.h"

CustomModel::CustomModel()
{
    qRegisterMetaType<Qt::Orientation>("Qt::Orientation");
}

int CustomModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent);
   return rows.count();
}

int CustomModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return header.count();
}

QVariant CustomModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(index.row() >= rows.count() || index.row() < 0)
        return QVariant();

    QString data;
    if(role == Qt::DisplayRole || role == Qt::EditRole)
     {
        return rows.at(index.row()).at(index.column());
     }
     return QVariant();
}

QVariant CustomModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
           return QVariant();

    if(orientation == Qt::Horizontal && section <= header.count()){
        return QVariant(header.at(section));
    }
    return QVariant();
}

void CustomModel::setHeader(Row header)
{
    this->header = header;
    this->setHeaderData(0, Qt::Horizontal, header.at(0));
    emit headerDataChanged(Qt::Horizontal, 0, header.count());
}

void CustomModel::setRows(QVector<Row> rows)
{
    this->rows = rows;
}
