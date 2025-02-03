#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcomepagewidget.h"
#include "loginwidget.h"
#include "parcourirdb.h"
#include "camwidget.h"

#include <QMessageBox>
#include <QtCore>
#include <QComboBox>
#include <QtGui>
#include <QTimer>
#include <QDate>
#include <QPixmap>
#include <QDebug>
#include <QtWidgets>
#include <QKeyEvent>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>
#include <QTextBrowser>
#include <QModelIndexList>
#include <QModelIndex>

#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QSerialPort>

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>


#include <QTableWidgetItem>


#include <QProcess>
#include <QFile>
#include <QTextStream>

#include <QCamera>
#include <QImage>
#include <QPixmap>
#include <QCameraDevice>
#include <QCameraFormat>
#include <QImageCapture>

#include <QImage>
#include <QMediaCaptureSession>
#include <QMediaMetaData>
#include <QMediaDevices>
#include <QByteArray>
#include <QBuffer>
#include<QVideoWidget>
#include<QVideoframe>
#include <QTimer>
#include <QCameraDevice>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QMediaPlayer>

#include <QVideoSink>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QVideoWidget>
#include <QMap>
#include <fstream> // Ajout de l'inclusion pour <fstream>
#include <iostream> // Ajout de l'inclusion pour <iostream>
#include <C:\Users\DARTY Sousse\Downloads\opencv\build\include\opencv2\opencv.hpp>
#include <C:\Users\DARTY Sousse\Downloads\opencv\build\include\opencv2\core.hpp>
#include <C:\Users\DARTY Sousse\Downloads\opencv\build\include\opencv2\highgui.hpp>
#include <C:\Users\DARTY Sousse\Downloads\opencv\build\include\opencv2\imgproc.hpp>
#include <C:\Users\DARTY Sousse\Downloads\opencv\build\include\opencv2\videoio.hpp>




        // Fonction pour effectuer la recherche dans la base de données
QList<QString> MainWindow::searchMedication(const QString &NAME) {
    QList<QString> searchResults;

    // Vérifier si une connexion avec le nom 'qt_sql_default_connection' existe déjà
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        // Supprimer la connexion existante
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    // Ajouter une nouvelle connexion à la base de données
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
    db.setDatabaseName(R"(C:\Users\DARTY Sousse\Desktop\PFEIMEN1\executable\database.db)");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database";
        return searchResults; // Retourner une liste vide en cas d'erreur
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM médicaments WHERE nom = :NAME");
    //query.bindValue(":nom",NAME);



    if (query.exec() && query.next()) {
        QStringList medicationInfoList;
        for (int i = 0; i < 20; ++i) {
            QString medicationInfo = query.value(i).toString();
            medicationInfoList << medicationInfo;
        }

        // Afficher le médicament dans la SearchLineEdit
        if (!medicationInfoList.isEmpty()) {
            ui->SearchLineEdit->setText(medicationInfoList.first());
        }

        // Ajouter les éléments à la liste
        ui->listWidget->addItems(medicationInfoList);
    } else {
        qDebug() << "Medication not found in the database";
    }

    db.close();

    return searchResults; // Retourner la liste des résultats
}


void createFilledTableWidget(QTableWidget *tableWidget) {
    // Connexion à la base de données
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(R"(C:\Users\DARTY Sousse\Desktop\PFEIMEN1\executable\database.db)");

    // Vérification de la connexion à la base de données
    if (!db.open()) {
        qDebug() << "Erreur: impossible de se connecter à la base de données";
        return;
    }

    // Exemple de requête SQL pour récupérer des données
    QSqlQuery query("SELECT * FROM médicaments");

    // Définition du nombre de colonnes du QTableWidget
    int columnCount = query.record().count(); // Nombre de colonnes

    // Configuration du nombre de colonnes du QTableWidget
    tableWidget->setColumnCount(columnCount);

    // Remplissage des noms de colonnes
    for (int column = 0; column < columnCount; ++column) {
        QTableWidgetItem *headerItem = new QTableWidgetItem(query.record().fieldName(column));
        tableWidget->setHorizontalHeaderItem(column, headerItem);
    }

    // Obtention du nombre de lignes
    int rowCount = 0;
    while (query.next()) {
        rowCount++;
    }
    query.first(); // Revenir au début du résultat

    // Configuration du nombre de lignes du QTableWidget
    tableWidget->setRowCount(rowCount);

    // Remplissage du QTableWidget à partir des données de la base de données
    int row = 0;
    while (query.next()) {
        for (int column = 0; column < columnCount; ++column) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(column).toString());
            tableWidget->setItem(row, column, item);
        }
        row++;
    }
}





int db(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //setupDatabaseConnection();
    //createTable();
    //insertData("Aspirin");
    //insertData("Paracetamol");
    //retrieveData();
    //closeDatabaseConnection();
    // Création du widget principal

    return a.exec();

}

// Déclarez un membre de votre classe MainWindow pour le modèle de données
MedicationTableModel *medicationModel;

QLabel *camLabel;
QCamera *camera;
QImageCapture *imageCapture;
QTimer *captureTimer;




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new QNetworkAccessManager(this))


{
    ui->setupUi(this);


    outputResult = ""; // Initialisation de outputResult
    ui->label_10->show();
    // Remplir le QTableWidget avec les données de la base de données
    createFilledTableWidget(ui->tableWidget);


    // Ajouter une nouvelle connexion à la base de données
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
    db.setDatabaseName(R"(C:\Users\DARTY Sousse\Desktop\PFEIMEN1\executable\database.db)");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database";

    }
    // Initialisez votre modèle ici
    //medicationModel = new MedicationTableModel(this);



    WelcomePageWidget *welcomePage = new WelcomePageWidget(this);
    ui->stackedWidget->addWidget(welcomePage); // Add the welcome page to index 1

    LoginWidget *loginWidget = new LoginWidget(this); // Renommer la variable locale pour éviter la confusion
    ui->stackedWidget->addWidget(loginWidget); // Ajouter le widget de connexion

    // Connecter le signal clicked() du bouton "welcome" à la fonction on_welcome_clicked()
    connect(ui->welcome, &QPushButton::clicked, this, &MainWindow::on_welcome_clicked);

    // Connecter le signal loginRequested du widget de connexion à la fonction on_Login_clicked()
    connect(loginWidget, &LoginWidget::loginRequested, this, &MainWindow::on_Login_clicked);

    // Connecter le signal clicked() du bouton de recherche à la fonction on_searchButton_clicked()
    connect(ui->SearchButton, &QPushButton::clicked, this, &MainWindow::on_SearchButton_clicked);

    connect(ui->SearchLineEdit, &QLineEdit::textEdited, this, &MainWindow::on_SearchLineEdit_textEdited);

    connect(imageCapture, &QImageCapture::imageCaptured, this, &MainWindow::on_imageCaptured);
    connect(ui->Envoyer, &QPushButton::clicked, this, &MainWindow::on_Envoyer_clicked);


    connect(ui->SearchLineEdit_2, &QLineEdit::textEdited, this, &MainWindow::on_SearchLineEdit_2_textEdited);
    connect(ui->AjouterMedicament, &QPushButton::clicked, this, &MainWindow::on_AjouterMedicament_clicked);


    // Créer le bouton de capture d'image
    //QPushButton *Capture = new QPushButton("Capturer Image", this);

    // Connecter le clic du bouton à la fonction de capture d'image
    //connect(Capture, &QPushButton::clicked, this, &MainWindow::on_Capture_clicked);

    // Connectez le signal imageCaptured au slot processCapturedImage
    connect(imageCapture, &QImageCapture::imageCaptured, this, &MainWindow::processCapturedImage);




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_b0_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b0");
    clickedButtons.append("b0");
    updateTextEdit();
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    qDebug() << "Index de la page actuellement affichée : " << arg1;
    if (arg1 == 0) {
        qDebug() << "Page 0 affichée";
        ui->textEdit->setText(clickedButtons.join(" "));
    }

}

void envoyerCommande(const QString &commande)
{
    // Trouver le port série disponible
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QSerialPort serialPort;

    // Parcourir les ports série disponibles
    for (const QSerialPortInfo &portInfo : ports) {
        qDebug() << "Port trouvé : " << portInfo.portName();

        // Utiliser le premier port série disponible
        serialPort.setPort(portInfo);

        // Ouvrir le port en lecture/écriture
        if (serialPort.open(QIODevice::ReadWrite)) {
            qDebug() << "Port ouvert avec succès";

            // Configurer les paramètres de communication
            serialPort.setBaudRate(QSerialPort::Baud9600);
            serialPort.setDataBits(QSerialPort::Data8);
            serialPort.setParity(QSerialPort::NoParity);
            serialPort.setStopBits(QSerialPort::OneStop);
            serialPort.setFlowControl(QSerialPort::NoFlowControl);

            // Envoyer la commande
            qint64 bytesWritten = serialPort.write(commande.toUtf8());
            if (bytesWritten == -1) {
                qDebug() << "Erreur lors de l'écriture sur le port série : " << serialPort.errorString();
            } else {
                qDebug() << "Commande envoyée avec succès";
            }

            // Fermer le port série
            serialPort.close();
            qDebug() << "Port fermé";
            break; // Sortir de la boucle une fois que le port a été trouvé et utilisé
        } else {
            qDebug() << "Impossible d'ouvrir le port : " << serialPort.errorString();
        }
    }
}


void MainWindow::on_b1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b1");
    clickedButtons.append("b1");
    updateTextEdit();
}


void MainWindow::on_b2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b2");
    clickedButtons.append("b2");
    updateTextEdit();
}

void MainWindow::updateTextEdit()
{
    // Affiche tous les boutons cliqués dans l'ordre
    ui->textEdit->setText(clickedButtons.join(" "));
}

void MainWindow::on_b3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b3");
    clickedButtons.append("b3");
    updateTextEdit();
}


void MainWindow::on_b4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b4");
    clickedButtons.append("b4");
    updateTextEdit();
}


void MainWindow::on_b5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b5");
    clickedButtons.append("b5");
    updateTextEdit();
}


void MainWindow::on_b6_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b6");
    clickedButtons.append("b6");
    updateTextEdit();
}


void MainWindow::on_b7_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b7");
    clickedButtons.append("b7");
    updateTextEdit();
}


void MainWindow::on_b8_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b8");
    clickedButtons.append("b8");
    updateTextEdit();
}


void MainWindow::on_b9_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b9");
    clickedButtons.append("b9");
    updateTextEdit();
}


void MainWindow::on_b10_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b10");
    clickedButtons.append("b10");
    updateTextEdit();
}


void MainWindow::on_Clear_clicked()
{
    if (!clickedButtons.isEmpty()) {
        clickedButtons.removeLast(); // Supprime le dernier élément de la liste
        updateTextEdit(); // Met à jour l'affichage
    }
}


void MainWindow::on_Clear_All_clicked()
{
    clickedButtons.clear(); // Efface tous les éléments de la liste
    updateTextEdit(); // Met à jour l'affichage
}

void MainWindow::on_welcome_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    LoginWidget *loginWidget = ui->stackedWidget->currentWidget()->findChild<LoginWidget*>("LoginWidget"); // Trouver l'instance du widget de connexion
    if(loginWidget) {
        ui->stackedWidget->setCurrentWidget(loginWidget); // Définir le widget de connexion comme widget actuel
    } else {
        qDebug() << "LoginWidget not found!";

    }
}





void MainWindow::on_Login_clicked()
{
    QString Username , password ;
    Username = ui -> lineEdit_username -> text() ;
    password = ui -> lineEdit_password -> text() ;


    // Vérifier si les champs de nom d'utilisateur et de mot de passe ne sont pas vides
    if (Username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir votre nom d'utilisateur et votre mot de passe.");
        return;
    }

    // Vérifier si le nom d'utilisateur ne dépasse pas 16 caractères
    if (Username.length() > 16) {
        QMessageBox::warning(this, "Erreur", "Le nom d'utilisateur ne peut pas dépasser 16 caractères.");
        return;
    }

    // Vérifier si le mot de passe est entre 4 et 12 caractères inclusivement
    if (password.length() < 4 || password.length() > 12) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe doit être entre 4 et 12 caractères.");
        return;
    }

    // Vérifier si le mot de passe contient au moins une lettre majuscule, une lettre minuscule,
    // un caractère spécial et un chiffre en utilisant des expressions régulières
    QRegularExpression uppercaseRegex("[A-Z]");
    QRegularExpression lowercaseRegex("[a-z]");
    QRegularExpression digitRegex("\\d");
    QRegularExpression specialCharRegex("[!@#$%^&*(),.?\":{}|<>]");

    if (!password.contains(uppercaseRegex) || !password.contains(lowercaseRegex) ||
        !password.contains(digitRegex) || !password.contains(specialCharRegex)) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe doit inclure au moins une lettre majuscule, une lettre minuscule, un chiffre et un caractère spécial.");
        return;
    }

    // Si toutes les validations sont réussies, effectuez la transition vers la prochaine page
    ui->stackedWidget->setCurrentIndex(5); // Changer l'index selon vos besoins

}


void MainWindow::on_Send_clicked()
{
    // Récupérer la commande saisie
    QString commande = ui->textEdit->toPlainText();

    // Envoyer la commande à la machine
    envoyerCommande(commande);
}


void MainWindow::afficherSuggestions(const QList<QString> &suggestions) {
    // Effacer le contenu actuel de la QListWidget
    ui->listWidget->clear();

    // Ajouter chaque suggestion à la QListWidget
    for (const QString &suggestion : suggestions) {
        ui->listWidget->addItem(suggestion);
    }
}



void MainWindow::on_SearchButton_clicked()
{
    // Récupérer le texte saisi dans la zone de recherche
    QString searchTerm = ui->SearchLineEdit->text();

    // Effectuer la recherche dans la base de données ou toute autre source de données
    QList<QString> searchResults = searchMedication(searchTerm);

    // Afficher les résultats de la recherche dans la zone de suggestions
    afficherSuggestions(searchResults);
}

// Code pour capturer le nom du médicament à partir de l'image et appeler la fonction de recherche
void captureMedicationNameFromImage() {
    QString medicationName = "NomDuMedicamentCapture"; // À remplacer par la logique de capture d'image
    //searchMedication(medicationName);
}

void MainWindow::on_b11_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b11");
    clickedButtons.append("b11");
    updateTextEdit();
}


void MainWindow::on_b12_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b12");
    clickedButtons.append("b12");
    updateTextEdit();
}
void MainWindow::on_b13_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b13");
    clickedButtons.append("b13");
    updateTextEdit();
}


void MainWindow::on_b14_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b14");
    clickedButtons.append("b14");
    updateTextEdit();
}


void MainWindow::on_b15_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b15");
    clickedButtons.append("b15");
    updateTextEdit();
}


void MainWindow::on_b16_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b16");
    clickedButtons.append("b16");
    updateTextEdit();
}


void MainWindow::on_b17_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b17");
    clickedButtons.append("b17");
    updateTextEdit();
}


void MainWindow::on_b18_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b18");
    clickedButtons.append("b18");
    updateTextEdit();
}


void MainWindow::on_b19_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b19");
    clickedButtons.append("b19");
    updateTextEdit();
}


void MainWindow::on_b20_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b20");
    clickedButtons.append("b20");
    updateTextEdit();
}


void MainWindow::on_b21_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b21");
    clickedButtons.append("b21");
    updateTextEdit();
}


void MainWindow::on_b22_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b22_2");
    clickedButtons.append("b22_2");
    updateTextEdit();
}




void MainWindow::on_b22_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b22");
    clickedButtons.append("b22");
    updateTextEdit();
}


void MainWindow::on_b23_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b23");
    clickedButtons.append("b23");
    updateTextEdit();
}


void MainWindow::on_b24_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b24");
    clickedButtons.append("b24");
    updateTextEdit();
}


void MainWindow::on_b25_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b25");
    clickedButtons.append("b25");
    updateTextEdit();
}


void MainWindow::on_b26_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b26");
    clickedButtons.append("b26");
    updateTextEdit();
}


void MainWindow::on_b27_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b27");
    clickedButtons.append("b27");
    updateTextEdit();
}


void MainWindow::on_b28_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b28");
    clickedButtons.append("b28");
    updateTextEdit();
}


void MainWindow::on_b29_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b29");
    clickedButtons.append("b29");
    updateTextEdit();
}


void MainWindow::on_b30_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b29");
    clickedButtons.append("b29");
    updateTextEdit();
}


void MainWindow::on_b31_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b30");
    clickedButtons.append("b30");
    updateTextEdit();
}


void MainWindow::on_b32_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b31");
    clickedButtons.append("b31");
    updateTextEdit();
}


void MainWindow::on_b33_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b32");
    clickedButtons.append("b32");
    updateTextEdit();
}


void MainWindow::on_b34_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b34");
    clickedButtons.append("b34");
    updateTextEdit();
}


void MainWindow::on_b35_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b35");
    clickedButtons.append("b35");
    updateTextEdit();
}


void MainWindow::on_b36_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b36");
    clickedButtons.append("b36");
    updateTextEdit();
}


void MainWindow::on_b37_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b37");
    clickedButtons.append("b37");
    updateTextEdit();
}


void MainWindow::on_b38_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b38");
    clickedButtons.append("b38");
    updateTextEdit();
}


void MainWindow::on_b39_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b39");
    clickedButtons.append("b39");
    updateTextEdit();
}


void MainWindow::on_b40_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b40");
    clickedButtons.append("b40");
    updateTextEdit();
}


void MainWindow::on_b41_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b41");
    clickedButtons.append("b41");
    updateTextEdit();
}


void MainWindow::on_b42_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b42");
    clickedButtons.append("b42");
    updateTextEdit();
}


void MainWindow::on_b43_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->setText("b43");
    clickedButtons.append("b43");
    updateTextEdit();
}

void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    // Vérifier si l'index de la cellule est valide
    if (index.isValid()) {
        // Récupérer les données de la cellule sélectionnée à partir du modèle
        QVariant cellData = medicationModel->data(index);

        // Vérifier si les données de la cellule sont valides
        if (cellData.isValid()) {
            // Convertir les données en QString (ou tout autre type approprié)
            QString cellText = cellData.toString();

            // Afficher le texte de la cellule dans une boîte de message par exemple
            QMessageBox::information(this, "Cellule activée", "Texte de la cellule : " + cellText);
        } else {
            // Afficher un message d'avertissement si les données de la cellule ne sont pas valides
            QMessageBox::warning(this, "Erreur", "Données de la cellule non valides !");
        }
    } else {
        // Afficher un message d'erreur si l'index de la cellule n'est pas valide
        QMessageBox::warning(this, "Erreur", "Index de cellule non valide !");
    }
}

void MainWindow::on_SearchLineEdit_textEdited(const QString &text) {
    // Vérifier si le texte est vide
    if (text.isEmpty()) {
        // Effacer les suggestions et quitter la fonction
        afficherSuggestions({});
        return;
    }

    // Effectuer une recherche dans la base de données avec le texte entré
    QList<QString> searchResults = searchMedicationByPrefix(text);

    // Afficher les résultats de la recherche dans la zone de suggestions
    afficherSuggestions(searchResults);
}

QList<QString> MainWindow::searchMedicationByPrefix(const QString &prefix) {
    QList<QString> searchResults;

    // Vérifier si une connexion avec le nom 'qt_sql_default_connection' existe déjà
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        // Supprimer la connexion existante
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    // Ajouter une nouvelle connexion à la base de données
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
    db.setDatabaseName(R"(C:\Users\DARTY Sousse\Desktop\PFEIMEN1\executable\database.db)");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database";
        return searchResults; // Retourner une liste vide en cas d'erreur
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM médicaments WHERE nom LIKE :prefix || '%'");
    query.bindValue(":prefix", prefix);

    if (query.exec()) {
        while (query.next()) {
            QString medicationInfo = query.value(1).toString(); // Assuming column 1 contains medication info
            // Ajouter le médicament trouvé à la liste des résultats
            searchResults.append(medicationInfo);
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    db.close();

    return searchResults; // Retourner la liste des résultats
}



void MainWindow::runPythonScript(const QString &scriptPath, const QStringList &arguments) {
    QProcess *process = new QProcess(this);

    // Définir le chemin du script Python à exécuter
    process->setProgram("python");
    process->setArguments(arguments);

    // Démarrer le processus
    process->start();

    // Vérifier si le processus a démarré avec succès
    if (!process->waitForStarted()) {
        qDebug() << "Erreur : Impossible de démarrer le script Python.";
        qDebug() << "QProcess error: " << process->errorString();
        delete process;
        return;
    }

    // Lire la sortie standard du processus
    connect(process, &QProcess::readyReadStandardOutput, [=]() {
        QByteArray output = process->readAllStandardOutput();
        qDebug() << "Sortie du script Python : " << output;
        ui->camicon->setText(QString::fromUtf8(output));
    });

    // Lire la sortie d'erreur du processus
    connect(process, &QProcess::readyReadStandardError, [=]() {
        QByteArray error = process->readAllStandardError();
        qDebug() << "Erreur du script Python : " << error;
    });

    // Connecter le signal de fin du processus
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus) {
                qDebug() << "Le script Python s'est terminé avec le code de sortie" << exitCode;
                process->deleteLater();
            });
}

void MainWindow::on_video_customContextMenuRequested(const QPoint&)
{
    // Exécuter le script Python
    QString scriptPath = "C:\\Users\\DARTY Sousse\\Desktop\\PFEIMEN1\\soft\\drugsinspectionLV.py";
    QString command = "python " + scriptPath;

    QProcess process;
    process.start(command);
    process.waitForFinished();

    // Lire la sortie du script Python
    QByteArray output = process.readAllStandardOutput();

    // Créer un QBuffer pour fournir les données au widget vidéo
    QBuffer *buffer = new QBuffer(&output, this);
    buffer->open(QIODevice::ReadOnly);

    // Définir les données vidéo brutes directement dans le widget vidéo existant
    ui->video->show();
    ui->video->setAttribute(Qt::WA_NoSystemBackground);
    ui->video->setAttribute(Qt::WA_TranslucentBackground);
    ui->video->setAttribute(Qt::WA_PaintOnScreen);
    ui->video->update();

    // Afficher les données vidéo brutes dans le widget vidéo existant
    ui->video->repaint();
}



void MainWindow::imageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);
    ui->msg->setText(tr("Captured \"%1\"").arg(QDir::toNativeSeparators(fileName)));
}
void MainWindow::startPythonScript(const QString &imagePath) {
    qDebug() << "Received imagePath from C++: " << imagePath;

    QString pythonScriptPath = "C:\\Users\\DARTY Sousse\\Desktop\\PFEIMEN1\\soft\\drugsinspectionLV.py";
    QStringList arguments;
    arguments << pythonScriptPath << imagePath;

    runPythonScript(pythonScriptPath, arguments);
}


void MainWindow::on_Capture_clicked() {
    // Capture de l'image
    imageCapture->capture();
}



void MainWindow::on_imageCaptured(int id, const QImage& preview) {
    Q_UNUSED(id);

    // Définition du répertoire de base pour enregistrer l'image
    QString baseDir = "C:\\Users\\DARTY Sousse\\Desktop\\PFEIMEN1\\soft\\imgcap\\image_with_camera_";
    QString fileName = baseDir + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".jpg";

    if (preview.save(fileName)) {
        ui->msg->setText(tr("Image saved to \"%1\"").arg(QDir::toNativeSeparators(fileName)));
        startPythonScript(fileName);
    } else {
        ui->msg->setText(tr("Failed to save image."));
    }
}



void MainWindow::on_camicon_linkActivated(const QString )
{
    // Chemin vers le fichier Python converti
    QString scriptPath = "C:\\Users\\DARTY Sousse\\Desktop\\PFEIMEN1\\soft\\drugsinspectionLV.py";
    QString output;

    // Exécuter le script Python et capturer la sortie
    QProcess process;
    process.start("python", QStringList() << scriptPath);
    if (process.waitForStarted() && process.waitForFinished()) {
        output = QString::fromUtf8(process.readAllStandardOutput());
    } else {
        output = "Erreur : Impossible d'exécuter le script Python.";
    }

    // Mettre à jour le contenu de SearchLineEdit avec les résultats du script Python
    ui->SearchLineEdit->setText(output);
}

void MainWindow::on_EditingFinished()
{
    // Définition du texte du QLabel
    ui->SearchLineEdit->setText(outputResult);
}






void MainWindow::processCapturedImage(int requestId, const QImage& img)
{
    Q_UNUSED(requestId);

    QImage scaledImage = img.scaled(ui->video->size(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation);

    ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));
    // Définition du répertoire de base pour enregistrer l'image
    QString baseDir = "C:\\Users\\DARTY Sousse\\Desktop\\PFEIMEN1\\soft\\imgcap\\image with camera";

    // Enregistrement de l'image capturée avec un nom de fichier unique basé sur la date et l'heure
    QString fileName = baseDir + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".jpg";
    bool saved = img.save(fileName);

    // Affichage d'un message indiquant si l'image a été enregistrée avec succès ou non
    if (saved) {
        ui->msg->setText(tr("Image saved to \"%1\"").arg(QDir::toNativeSeparators(fileName)));
        // Appel de la fonction pour démarrer le script Python avec le chemin de l'image comme argument
        startPythonScript(fileName);
    }
    else {
        ui->msg->setText(tr("Failed to save image."));
    }
}

void MainWindow::fetchMedicationDataByPrefix(const QString &prefix, QTableWidget *tableWidget) {
    // Vérifier si une connexion avec le nom 'qt_sql_default_connection' existe déjà
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        // Supprimer la connexion existante
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    // Ajouter une nouvelle connexion à la base de données
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
    db.setDatabaseName(R"(C:\Users\DARTY Sousse\Desktop\PFEIMEN1\executable\database.db)");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database";
        return; // Sortir de la fonction en cas d'erreur
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM médicaments WHERE nom LIKE :prefix || '%'");
    query.bindValue(":prefix", prefix);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        db.close();
        return; // Sortir de la fonction en cas d'erreur
    }

    // Obtenir le nombre de colonnes de la requête
    int columnCount = query.record().count();

    // Vider le contenu de la table existante
    tableWidget->clearContents();

    // Remplissage du QTableWidget à partir des données de la base de données
    int row = 0;
    while (query.next()) {
        if (row >= tableWidget->rowCount()) {
            tableWidget->setRowCount(row + 1);
        }
        for (int column = 0; column < columnCount; ++column) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(column).toString());
            tableWidget->setItem(row, column, item);
        }
        row++;
    }

    db.close();
}


void MainWindow::on_SearchLineEdit_2_textEdited(const QString &arg1)
{
    // Rechercher le médicament dans la base de données et remplir le QTableWidget
    fetchMedicationDataByPrefix(arg1, ui->tableWidget);
}


void MainWindow::on_SearchI_clicked()
{
    // Récupérer le texte saisi dans la zone de recherche
    QString searchTerm = ui->SearchLineEdit_2->text();

    // Effectuer la recherche dans la base de données ou toute autre source de données
    QList<QString> searchResults = searchMedication(searchTerm);

    // Afficher les résultats de la recherche dans la zone de suggestions
    afficherSuggestions(searchResults);
}

void MainWindow::on_AjouterMedicament_clicked()
{
    // Ouvrir une boîte de dialogue pour l'entrée manuelle ou exécuter le script Python
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Ajouter Médicament", "Voulez-vous ajouter un médicament manuellement ?",
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        // Entrée manuelle
        QDialog dialog(this);
        QFormLayout form(&dialog);

        // Création des champs d'entrée pour les détails du médicament
        QLineEdit *CodemédicamentEdit = new QLineEdit(&dialog);
        form.addRow("Code médicament:", CodemédicamentEdit);

        QLineEdit *nomEdit = new QLineEdit(&dialog);
        form.addRow("nom:", nomEdit);

        QLineEdit *dosageEdit = new QLineEdit(&dialog);
        form.addRow("dosage:", dosageEdit);

        QLineEdit *QuantitéaustockEdit = new QLineEdit(&dialog);
        form.addRow("Quantité au stock:", QuantitéaustockEdit);

        QLineEdit *prixunitaireEdit = new QLineEdit(&dialog);
        form.addRow("prix unitaire:", prixunitaireEdit);

        QLineEdit *DatedexpirationEdit = new QLineEdit(&dialog);
        form.addRow("Date d'expiration:", DatedexpirationEdit);

        QLineEdit *RemarqueEdit = new QLineEdit(&dialog);
        form.addRow("Remarque:", RemarqueEdit);

        QLineEdit *positionEdit = new QLineEdit(&dialog);
        form.addRow("position:", positionEdit);

        QLineEdit *quantitéEdit = new QLineEdit(&dialog);
        form.addRow("quantité:", quantitéEdit);

        // Ajout des boutons OK et Annuler
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

        // Si l'utilisateur accepte le dialogue
        if (dialog.exec() == QDialog::Accepted) {
            QString Codemédicament = CodemédicamentEdit->text();
            QString nom = nomEdit->text();
            QString dosage = dosageEdit->text();
            int Quantitéaustock = QuantitéaustockEdit->text().toInt();
            QString prixunitaire = prixunitaireEdit->text();
            QString Datedexpiration = DatedexpirationEdit->text();
            QString Remarque = RemarqueEdit->text();
            QString position = positionEdit->text();
            int quantité = quantitéEdit->text().toInt();

            // Ajouter le médicament dans Firebase
            QString apiKey = "AIzaSyCfk4dGfTuJ3hWHwOIgvfYvV4Uwjn9Kz7I";
            QString urlStr = QString("https://djin-78525-default-rtdb.firebaseio.com/medications.json?auth=%1").arg(apiKey);
            QUrl url(urlStr);
            QNetworkRequest request(url);

            QJsonObject json;
            json["code"] = Codemédicament;
            json["nom"] = nom;
            json["dosage"] = dosage;
            json["Quantité au stock"] = Quantitéaustock;
            json["prix unitaire"] = prixunitaire;
            json["Date d'expiration"] = Datedexpiration;
            json["Remarque"] = Remarque;
            json["position"] = position;
            json["quantité"] = quantité;
            QJsonDocument doc(json);

            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            QNetworkReply *reply = networkManager->put(request, doc.toJson());

            connect(reply, &QNetworkReply::finished, [this, reply, Codemédicament, nom, dosage, Quantitéaustock, prixunitaire, Datedexpiration, Remarque, position, quantité]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QMessageBox::information(this, "Succès", "Médicament ajouté avec succès dans Firebase");

                    // Ajouter le médicament dans le tableau de l'interface utilisateur
                    int row = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(row);
                    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(Codemédicament));
                    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(nom));
                    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(dosage));
                    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(Quantitéaustock)));
                    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(prixunitaire));
                    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(Datedexpiration));
                    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(Remarque));
                    ui->tableWidget->setItem(row, 7, new QTableWidgetItem(position));
                    ui->tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(quantité)));

                    // Ajouter le médicament dans la base de données SQLite
                    QSqlQuery query;
                    query.prepare("INSERT INTO médicaments ('Code médicament', nom, dosage, `Quantité au stock`, `prix unitaire`, `Date d'expiration`, Remarque, position, quantité) "
                                  "VALUES (:Codemédicament, :nom, :dosage, :Quantitéaustock, :prixunitaire, :Datedexpiration, :Remarque, :position, :quantité)");
                    query.bindValue(":code", Codemédicament);
                    query.bindValue(":nom", nom);
                    query.bindValue(":dosage", dosage);
                    query.bindValue(":Quantitéaustock", Quantitéaustock);
                    query.bindValue(":prixunitaire", prixunitaire);
                    query.bindValue(":Datedexpiration", Datedexpiration);
                    query.bindValue(":Remarque", Remarque);
                    query.bindValue(":position", position);
                    query.bindValue(":quantité", quantité);

                    if (!query.exec()) {
                        qDebug() << "Erreur d'insertion dans la base de données:" << query.lastError();
                        QMessageBox::critical(this, "Erreur de base de données", "Échec de l'ajout du médicament dans la base de données: " + query.lastError().text());
                    }

                } else {
                    QMessageBox::critical(this, "Erreur", "Échec de l'ajout du médicament dans Firebase: " + reply->errorString());
                }
                reply->deleteLater();
            });
        }
    } else if (reply == QMessageBox::No) {
        // Créer un processus pour exécuter le script Python
        QProcess *process = new QProcess(this);
        QString program = "python";
        QStringList arguments;
        arguments << "C:\\Users\\DARTY Sousse\\Desktop\\PFEIMEN1\\soft\\drugsinspectionLV.py";

        //QProcess *process = new QProcess(this);
        process->setProgram(program);
        process->setArguments(arguments);

        connect(process, &QProcess::finished, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitStatus == QProcess::NormalExit && exitCode == 0) {
                QByteArray output = process->readAllStandardOutput();
                QJsonDocument doc = QJsonDocument::fromJson(output);
                QJsonObject obj = doc.object();

                QString Codemédicament = obj["Code médicament"].toString();
                QString nom = obj["nom"].toString();
                QString dosage = obj["dosage"].toString();
                int Quantitéaustock = obj["Quantité au stock"].toInt();
                QString prixunitaire = obj["prix unitaire"].toString();
                QString Datedexpiration = obj["Date d'expiration"].toString();
                QString Remarque = obj["Remarque"].toString();
                QString position = obj["position"].toString();
                int quantité = obj["quantité"].toInt();

                // Ajouter le médicament dans Firebase
                QString apiKey = "AIzaSyCfk4dGfTuJ3hWHwOIgvfYvV4Uwjn9Kz7I";
                QString urlStr = QString("https://djin-78525-default-rtdb.firebaseio.com/medications.json?auth=%1").arg(apiKey);
                QUrl url(urlStr);
                QNetworkRequest request(url);

                QJsonObject json;
                json["Code médicament"] = Codemédicament;
                json["nom"] = nom;
                json["dosage"] = dosage;
                json["Quantité au stock"] = Quantitéaustock;
                json["prix unitaire"] = prixunitaire;
                json["Date d'expiration"] = Datedexpiration;
                json["Remarque"] = Remarque;
                json["position"] = position;
                json["quantité"] = quantité;

                QJsonDocument docToSend(json);
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QNetworkReply *reply = networkManager->put(request, docToSend.toJson());

                connect(reply, &QNetworkReply::finished, [this, reply, Codemédicament, nom, dosage, Quantitéaustock, prixunitaire, Datedexpiration, Remarque, position, quantité]() {
                    if (reply->error() == QNetworkReply::NoError) {
                        QMessageBox::information(this, "Succès", "Médicament ajouté avec succès dans Firebase");

                        // Ajouter le médicament dans le tableau de l'interface utilisateur
                        int row = ui->tableWidget->rowCount();
                        ui->tableWidget->insertRow(row);
                        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(Codemédicament));
                        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(nom));
                        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(dosage));
                        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(Quantitéaustock)));
                        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(prixunitaire));
                        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(Datedexpiration));
                        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(Remarque));
                        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(position));
                        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(quantité)));

                        // Ajouter le médicament dans la base de données SQLite
                        QSqlQuery query;
                        query.prepare("INSERT INTO médicaments ('Code médicament', nom, dosage, `Quantité au stock`, `prix unitaire`, `Date d'expiration`, Remarque, position, quantité) "
                                      "VALUES (:Codemédicament, :nom, :dosage, :Quantitéaustock, :prixunitaire, :Datedexpiration, :Remarque, :position, :quantité)");
                        query.bindValue(":Codemédicament", Codemédicament);
                        query.bindValue(":nom", nom);
                        query.bindValue(":dosage", dosage);
                        query.bindValue(":Quantitéaustock", Quantitéaustock);
                        query.bindValue(":prixunitaire", prixunitaire);
                        query.bindValue(":Datedexpiration", Datedexpiration);
                        query.bindValue(":Remarque", Remarque);
                        query.bindValue(":position", position);
                        query.bindValue(":quantité", quantité);

                        if (!query.exec()) {
                            qDebug() << "Erreur d'insertion dans la base de données:" << query.lastError();
                            QMessageBox::critical(this, "Erreur de base de données", "Échec de l'ajout du médicament dans la base de données: " + query.lastError().text());
                        }

                    } else {
                        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du médicament dans Firebase: " + reply->errorString());
                    }
                    reply->deleteLater();
                });

                // Afficher l'image dans le QLabel
                QByteArray imageBytes = QByteArray::fromBase64(obj["image"].toString().toUtf8());
                QPixmap pixmap;
                pixmap.loadFromData(imageBytes);
                //ui->video->setPixmap(pixmap.scaled(ui->video->size(), Qt::KeepAspectRatio));

            } else {
                QMessageBox::critical(this, "Erreur", "Échec de l'exécution du script Python");
            }
            process->deleteLater();
        });

        process->start();
    } else if (reply == QMessageBox::Cancel) {
        // L'utilisateur a choisi Cancel, ne rien faire
        return;
    }
}


int MainWindow::getStockQuantity(const QString &medicationName) {
    QSqlQuery query;
    query.prepare("SELECT `Quantité au stock` FROM médicaments WHERE nom = :nom");
    query.bindValue(":nom", medicationName);

    if (!query.exec() || !query.next()) {
        qDebug() << "Query Error: " << query.lastError();
        return -1;
    }

    return query.value("Quantité au stock").toInt();
}

bool MainWindow::updateStockQuantity(const QString &medicationName, int newStockQuantity) {
    QSqlQuery query;
    query.prepare("UPDATE médicaments SET `Quantité au stock` = :newStockQuantity WHERE nom = :nom");
    query.bindValue(":newStockQuantity", newStockQuantity);
    query.bindValue(":nom", medicationName);

    if (!query.exec()) {
        qDebug() << "Query Error: " << query.lastError();
        return false;
    }

    return true;
}



void MainWindow::on_Envoyer_clicked()
{
    // Obtenir la ligne actuellement sélectionnée
    int row = ui->tableWidget->currentRow();

    // Indices des colonnes pertinentes
    int quantitéColumnIndex = 8;
    int nameColumnIndex = 1;
    int positionColumnIndex = 7;
    int QuantitéaustockColumnIndex = 3;

    // Vérifier si une ligne est sélectionnée
    if (row < 0) {
        QMessageBox::warning(this, "Input Error", "Please select a row");
        return;
    }

    // Récupérer les items des colonnes pertinentes pour la ligne sélectionnée
    QTableWidgetItem *quantitéItem = ui->tableWidget->item(row, quantitéColumnIndex);
    QTableWidgetItem *nameItem = ui->tableWidget->item(row, nameColumnIndex);
    QTableWidgetItem *positionItem = ui->tableWidget->item(row, positionColumnIndex);
    QTableWidgetItem *QuantitéaustockItem = ui->tableWidget->item(row, QuantitéaustockColumnIndex);

    // Vérifier si tous les items requis sont présents
    if (!quantitéItem || !nameItem || !positionItem || !QuantitéaustockItem) {
        QMessageBox::warning(this, "Input Error", "One or more required items not found");
        return;
    }

    // Récupérer et valider la quantité
    QString quantité = quantitéItem->text();
    if (quantité.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Quantity is empty");
        return;
    }

    bool ok;
    int quantity = quantité.toInt(&ok);
    if (!ok || quantity <= 0) {
        QMessageBox::warning(this, "Input Error", "Quantity must be a positive number");
        return;
    }

    // Récupérer le nom du médicament et la position
    QString medicationName = nameItem->text();
    QString position = positionItem->text();

    // Obtenir la quantité actuelle en stock de la base de données
    int Quantitéaustock = getStockQuantity(medicationName);
    if (Quantitéaustock < 0) {
        QMessageBox::critical(this, "Database Error", "Could not retrieve stock quantity for " + medicationName);
        return;
    }

    // Vérifier si la quantité à distribuer est disponible en stock
    if (quantity > Quantitéaustock) {
        QMessageBox::warning(this, "Input Error", "Distributed quantity exceeds stock quantity");
        return;
    }

    // Calculer la nouvelle quantité en stock
    int newStockQuantity = Quantitéaustock - quantity;

    // Créer la chaîne to_espp
    QString to_espp;
    for (int i = 0; i < quantity; ++i) {
        to_espp += position;
    }

    // Envoyer les données à Firebase si to_espp n'est pas vide
    if (!to_espp.isEmpty()) {
        QString apiKey = "AIzaSyCfk4dGfTuJ3hWHwOIgvfYvV4Uwjn9Kz7I";
        QString urlStr = QString("https://djin-78525-default-rtdb.firebaseio.com/espsend.json?auth=%1").arg(apiKey);
        QUrl url(urlStr);
        QNetworkRequest request(url);

        QJsonObject json;
        json["to_espp"] = to_espp;
        QJsonDocument doc(json);

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = networkManager->put(request, doc.toJson());

        // Gestion de la réponse de la requête Firebase
        connect(reply, &QNetworkReply::finished, [this, reply, to_espp, medicationName, newStockQuantity, QuantitéaustockItem]() {
            if (reply->error() == QNetworkReply::NoError) {
                QMessageBox::information(this, "Succès", "Positions des médicaments envoyées : " + to_espp);

                // Mise à jour de la quantité en stock dans la base de données
                if (updateStockQuantity(medicationName, newStockQuantity)) {
                    // Mettre à jour l'interface utilisateur avec la nouvelle quantité en stock
                    QuantitéaustockItem->setText(QString::number(newStockQuantity));
                } else {
                    QMessageBox::critical(this, "Database Error", "Failed to update stock quantity for " + medicationName);
                }
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de l'envoi des données : " + reply->errorString());
            }
            reply->deleteLater();
        });
    } else {
        QMessageBox::warning(this, "Attention", "Aucun médicament sélectionné.");
    }

    // Ne pas effacer le contenu du tableau
}
