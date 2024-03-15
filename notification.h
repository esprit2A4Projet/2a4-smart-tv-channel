#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QDate>
#include <QModelIndex>
#include <QStringListModel>
#include <algorithm>


// Créez un modèle personnalisé pour trier les messages de notification
class SortedStringListModel : public QStringListModel {
public:
    SortedStringListModel(QObject *parent = nullptr) : QStringListModel(parent) {}

    // Surchargez la méthode data pour trier les données
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole) {
            return sortedStrings.at(index.row());
        }
        return QVariant();
    }

    // Ajoutez une méthode pour trier les données
    void sortData() {
        std::sort(sortedStrings.begin(), sortedStrings.end(), [](const QString &a, const QString &b) {
            // Comparez les dates dans l'ordre décroissant
            return QDate::fromString(a.split(":").back().trimmed(), "yyyy-MM-dd HH:mm:ss") >
                   QDate::fromString(b.split(":").back().trimmed(), "yyyy-MM-dd HH:mm:ss");
        });
    }

    // Ajoutez une méthode pour définir les données triées
    void setSortedStrings(const QStringList &strings) {
        sortedStrings = strings;
        sortData();
    }

private:
    QStringList sortedStrings;
};





#endif // NOTIFICATION_H
