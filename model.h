#ifndef MODEL_H
#define MODEL_H

#include <QAbstractTableModel>

class CustomModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    using Row = QVector<QString>;

public:
    CustomModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    void setHeader(Row header);

public slots:
    void setRows(QVector<Row> rows);

private:
    QVector<Row> rows;
    Row header;
};

#endif // MODEL_H
