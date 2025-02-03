#ifndef PARCOURIRDB_H
#define PARCOURIRDB_H
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <QList>

// Définissez votre propre classe de modèle de données
class MedicationTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit MedicationTableModel(QObject *parent = nullptr)
        : QAbstractTableModel(parent) {}

    // Implémentez rowCount() pour renvoyer le nombre de lignes
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent)
        return medications.size();
    }


    // Implémentez data() pour fournir les données à afficher
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= medications.size())
            return QVariant();

        if (role == Qt::DisplayRole) {
            // Renvoie les données appropriées selon l'index
            return medications.at(index.row());
        }
        return QVariant();
    }

    // Méthode pour mettre à jour les données du modèle
    void updateData(const QList<QString> &data) {
        beginResetModel();
        medications = data;
        endResetModel();
    }

private:
    QList<QString> medications; // Les données à afficher dans le modèle
    MedicationTableModel *medicationModel;
};

#endif // PARCOURIRDB_H





