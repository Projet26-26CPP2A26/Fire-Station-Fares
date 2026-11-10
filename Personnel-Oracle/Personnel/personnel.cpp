#include "personnel.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

Personnel::Personnel()
{
    m_id = 0;
    m_missionsEnCours = 0;
}

Personnel::Personnel(int id, const QString &nom, const QString &prenom,
                 const QString &grade, const QString &poste, const QString &telephone,
                 const QString &email, const QString &motDePasse,
                 const QString &disponibilite, int missionsEnCours)
{
    m_id = id;
    m_nom = nom;
    m_prenom = prenom;
    m_grade = grade;
    m_poste = poste;
    m_telephone = telephone;
    m_email = email;
    m_motDePasse = motDePasse;
    m_disponibilite = disponibilite;
    m_missionsEnCours = missionsEnCours;
}

QString Personnel::idTexte() const
{
    return QString("P%1").arg(m_id, 3, 10, QChar('0'));
}

// ------------------------------------------------------------------
//  CREATE : l'id est généré par la séquence seq_personnel
// ------------------------------------------------------------------
bool Personnel::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO personnel (id_personnel, nom, prenom, grade, poste, telephone, email, disponibilite) "
                  "VALUES (seq_personnel.NEXTVAL, :nom, :prenom, :grade, :poste, :telephone, :email, :disponibilite)");
    query.bindValue(":nom", m_nom);
    query.bindValue(":prenom", m_prenom);
    query.bindValue(":grade", m_grade);
    query.bindValue(":poste", m_poste);
    query.bindValue(":telephone", m_telephone);
    query.bindValue(":email", m_email);
    query.bindValue(":disponibilite", m_disponibilite);

    if (!query.exec()) {
        qDebug() << "Erreur ajouter :" << query.lastError().text();
        return false;
    }
    return true;
}

// ------------------------------------------------------------------
//  UPDATE
// ------------------------------------------------------------------
bool Personnel::modifier() const
{
    QSqlQuery query;
    query.prepare("UPDATE personnel SET nom = :nom, prenom = :prenom, grade = :grade, poste = :poste, "
                  "telephone = :telephone, email = :email, disponibilite = :disponibilite "
                  "WHERE id_personnel = :id");
    query.bindValue(":nom", m_nom);
    query.bindValue(":prenom", m_prenom);
    query.bindValue(":grade", m_grade);
    query.bindValue(":poste", m_poste);
    query.bindValue(":telephone", m_telephone);
    query.bindValue(":email", m_email);
    query.bindValue(":disponibilite", m_disponibilite);
    query.bindValue(":id", m_id);

    if (!query.exec()) {
        qDebug() << "Erreur modifier :" << query.lastError().text();
        return false;
    }
    return true;
}

// ------------------------------------------------------------------
//  DELETE
// ------------------------------------------------------------------
bool Personnel::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM personnel WHERE id_personnel = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur supprimer :" << query.lastError().text();
        return false;
    }
    return true;
}

// ------------------------------------------------------------------
//  READ (version "atelier" : modèle pour un QTableView)
// ------------------------------------------------------------------
QSqlQueryModel* Personnel::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT id_personnel, nom, prenom, grade, poste, telephone, email, disponibilite "
                    "FROM personnel ORDER BY id_personnel");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Grade"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Disponibilité"));
    return model;
}

// ------------------------------------------------------------------
//  READ (liste d'objets, utilisée par le tableau de l'application)
//  Le nombre de missions en cours est calculé depuis mission_personnel.
// ------------------------------------------------------------------
QList<Personnel> Personnel::lireTous()
{
    QList<Personnel> liste;
    QSqlQuery query;
    query.prepare("SELECT p.id_personnel, p.nom, p.prenom, p.grade, p.poste, p.telephone, p.email, p.disponibilite, "
                  "(SELECT COUNT(*) FROM mission_personnel mp "
                  "   JOIN mission m ON m.id_mission = mp.id_mission "
                  "  WHERE mp.id_personnel = p.id_personnel AND m.etat = 'En cours') "
                  "FROM personnel p ORDER BY p.id_personnel");
    if (!query.exec()) {
        qDebug() << "Erreur lireTous :" << query.lastError().text();
        return liste;
    }
    while (query.next()) {
        liste.append(Personnel(query.value(0).toInt(),
                               query.value(1).toString(),
                               query.value(2).toString(),
                               query.value(3).toString(),
                               query.value(4).toString(),
                               query.value(5).toString(),
                               query.value(6).toString(),
                               QString(),                       // pas de mot de passe en base
                               query.value(7).toString(),
                               query.value(8).toInt()));
    }
    return liste;
}

int Personnel::charge() const
{
    int c = m_missionsEnCours * 20; // 20 % par mission
    if (c > 100)
        c = 100;
    return c;
}

QString Personnel::nomComplet() const
{
    return m_prenom + " " + m_nom;
}

// Du grade le plus élevé au moins élevé (sert aussi pour le tri)
QStringList Personnel::grades()
{
    QStringList liste;
    liste << "Colonel" << "Commandant" << "Capitaine" << "Lieutenant"
          << "Adjudant" << "Sergent" << "Caporal" << "Sapeur";
    return liste;
}

QStringList Personnel::postes()
{
    QStringList liste;
    liste << "Chef de garde" << "Intervention" << "Secours" << "Logistique" << "Administration";
    return liste;
}

QStringList Personnel::disponibilites()
{
    QStringList liste;
    liste << "Disponible" << "En mission" << "En congé";
    return liste;
}

int Personnel::rangGrade(const QString &grade)
{
    int i = grades().indexOf(grade);
    if (i < 0)
        i = grades().size();
    return i;
}
