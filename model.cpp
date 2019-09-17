#include "model.h"

CustomModel::CustomModel()
{

}

int CustomModel::rowCount(const QModelIndex &parent) const
{
    return 10;
}

int CustomModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant CustomModel::data(const QModelIndex &index, int role) const
{
   // Q_UNUSED(role);
    if (!index.isValid()) return QVariant();
    return QVariant("Some data");
}
