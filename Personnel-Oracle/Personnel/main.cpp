#include "gsmartfirestation.h"
#include "connection.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Connexion unique à Oracle (Singleton)
    Connection &c = Connection::createInstance();
    if (!c.createConnection()) {
        QMessageBox::critical(nullptr, "Base de données",
                              "Échec de la connexion à Oracle.\nVérifiez connection.cpp (source ODBC, utilisateur, mot de passe).");
        return 1;
    }

    // Style Fusion + feuille de style style.qss (charte graphique)
    a.setStyle(QStyleFactory::create("Fusion"));
    QFile fichierStyle(":/style/style.qss");
    if (fichierStyle.open(QFile::ReadOnly | QFile::Text))
        a.setStyleSheet(QString::fromUtf8(fichierStyle.readAll()));

    GSmartFireStation fenetre;
    fenetre.show();
    return a.exec();
}
