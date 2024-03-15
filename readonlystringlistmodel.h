#ifndef READONLYSTRINGLISTMODEL_H
#define READONLYSTRINGLISTMODEL_H

#include <QStringListModel>

class ReadOnlyStringListModel : public QStringListModel {
public:
    explicit ReadOnlyStringListModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // READONLYSTRINGLISTMODEL_H
