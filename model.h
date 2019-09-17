#ifndef MODEL_H
#define MODEL_H
#include <QAbstractTableModel>

class CustomModel : public QAbstractTableModel
{
public:
    explicit CustomModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // MODEL_H