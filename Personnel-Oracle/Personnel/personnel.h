#ifndef PERSONNEL_H
#define PERSONNEL_H

#include <QString>
#include <QStringList>
#include <QList>

class QSqlQueryModel;

// Entité "Personnel" (Module 1 : Gestion du personnel)
// = couche MODELE : c'est ici (et seulement ici) que se trouvent les requêtes SQL.
class Personnel
{
public:
    Personnel();
    Personnel(int id, const QString &nom, const QString &prenom,
              const QString &grade, const QString &poste, const QString &telephone,
              const QString &email, const QString &motDePasse,
              const QString &disponibilite, int missionsEnCours);

    // Getters
    int id() const { return m_id; }
    QString idTexte() const;                 // affichage : P001, P002...
    QString nom() const { return m_nom; }
    QString prenom() const { return m_prenom; }
    QString grade() const { return m_grade; }
    QString poste() const { return m_poste; }
    QString telephone() const { return m_telephone; }
    QString email() const { return m_email; }
    QString motDePasse() const { return m_motDePasse; }   // non stocké en base (hors modèle validé)
    QString disponibilite() const { return m_disponibilite; }
    int missionsEnCours() const { return m_missionsEnCours; }

    // Setters
    void setId(int v) { m_id = v; }
    void setNom(const QString &v) { m_nom = v; }
    void setPrenom(const QString &v) { m_prenom = v; }
    void setGrade(const QString &v) { m_grade = v; }
    void setPoste(const QString &v) { m_poste = v; }
    void setTelephone(const QString &v) { m_telephone = v; }
    void setEmail(const QString &v) { m_email = v; }
    void setMotDePasse(const QString &v) { m_motDePasse = v; }
    void setDisponibilite(const QString &v) { m_disponibilite = v; }
    void setMissionsEnCours(int v) { m_missionsEnCours = v; }

    // ----- CRUD (requêtes préparées) -----
    bool ajouter();                          // CREATE
    bool modifier() const;                   // UPDATE
    static bool supprimer(int id);           // DELETE
    static QSqlQueryModel* afficher();       // READ (modèle pour un QTableView)
    static QList<Personnel> lireTous();      // READ (liste d'objets pour le tableau de l'application)

    // Métier : charge opérationnelle en % (chaque mission en cours = 20 %, maximum 100 %)
    int charge() const;
    QString nomComplet() const;

    // Listes de valeurs autorisées
    static QStringList grades();
    static QStringList postes();
    static QStringList disponibilites();
    static int rangGrade(const QString &grade);

private:
    int m_id;
    QString m_nom;
    QString m_prenom;
    QString m_grade;
    QString m_poste;
    QString m_telephone;
    QString m_email;
    QString m_motDePasse;
    QString m_disponibilite;
    int m_missionsEnCours;
};

#endif // PERSONNEL_H
