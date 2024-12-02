#ifndef HISTORIQUE_H
#define HISTORIQUE_H




#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

class Historique {
public:
    Historique();  // Constructeur par défaut

    // Méthode pour ajouter une action à l'historique
    bool ajouter(const QString &action, const QString &details);

    // Méthode pour afficher l'historique dans un QSqlQueryModel
    QSqlQueryModel* afficher();

private:
    int idH;  // Identifiant de l'historique (non utilisé dans cet exemple, mais extensible)
};




#endif // HISTORIQUE_H
