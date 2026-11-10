#include <QApplication>
#include <QFile>
#include <QIcon>
#include <QMessageBox>
#include <QStyleFactory>

#include "personnel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/icon.png"));

    // Style Fusion + feuille de style style.qss (charte graphique)
    a.setStyle(QStyleFactory::create("Fusion"));
    QFile fichierStyle(":/style/style.qss");
    if (fichierStyle.open(QFile::ReadOnly | QFile::Text))
        a.setStyleSheet(QString::fromUtf8(fichierStyle.readAll()));

    // 1) Connexion unique à Oracle (Singleton)
    Connection &c = Connection::createInstance();
    if (!c.createConnection()) {
        QMessageBox::critical(nullptr, "Base de données",
                              "Échec de la connexion à Oracle.\n"
                              "Vérifiez personnel.cpp (source ODBC, utilisateur, mot de passe).");
        return 1;
    }

    // 2) Authentification, puis fenêtre principale
    MainWindow w;
    if (!w.connexion())
        return 0;
    w.show();
    return a.exec();
}
