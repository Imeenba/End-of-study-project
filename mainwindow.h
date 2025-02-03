#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

#include <QAbstractItemModel>
#include "QMessageBox"
#include<QtSql>
#include<QtDebug>
#include<QFileInfo>
#include <QTimer>
#include <QDate>
#include <QDebug>
#include <QtWidgets>
#include <QSerialPort>

#include <QVideoSink>
#include <QVideoWidget>
#include<QMediaDevices>
#include <QCameraDevice>
#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QCamera>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QMediaPlayer>
#include <QSettings>
#include<QtCharts>
#include<QChartView>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QLineEdit>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QMainWindow>
#include <QSplitter>
#include <QListWidget>
#include <QChartView>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QTimer>

#include <C:\Users\DARTY Sousse\Downloads\opencv\build\include\opencv2\opencv.hpp>
#include <C:\Users\DARTY Sousse\Downloads\opencv\build\include\opencv2\core.hpp>
#include <C:\Users\DARTY Sousse\Downloads\opencv\build\include\opencv2\highgui.hpp>
#include <C:\Users\DARTY Sousse\Downloads\opencv\build\include\opencv2\imgproc.hpp>
#include <C:\Users\DARTY Sousse\Downloads\opencv\build\include\opencv2\videoio.hpp>
#include <QVideoWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Définition de la structure Drug avant la classe MainWindow
struct Drug {
    QString name;
    int quantity;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    QList<QString> searchMedication(const QString &medicationName);

    // Déclaration des fonctions de recherche et d'affichage de suggestions
    QList<QString> rechercheMedicaments(const QString &searchTerm);
    void afficherSuggestions(const QList<QString> &suggestions);

    void setupDatabaseConnection();
    void createTable();
    void insertData(const QString &data);
    void retrieveData();
    void closeDatabaseConnection();
    QList<QString> searchMedicationByPrefix(const QString &prefix);

    void processCapturedImage(int requestId, const QImage& img);
    void imageSaved(int id, const QString &fileName);
    void startPythonScript(const QString &imagePath);
    void loadMedicaments();
    void fetchMedicationDataByPrefix(const QString &prefix, QTableWidget *tableWidget) ;
    void loadDataFromDatabase();
    bool updateStockQuantity(const QString &Quantitéaustock, int quantité) ;
    int  getStockQuantity(const QString &medicationName);


private slots:
    void on_b0_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_b1_clicked();

    void on_b2_clicked();

    void on_b3_clicked();

    void on_b4_clicked();

    void on_b5_clicked();

    void on_b6_clicked();

    void on_b7_clicked();

    void on_b8_clicked();

    void on_b9_clicked();

    void on_b10_clicked();

    void on_Clear_clicked();

    void on_Clear_All_clicked();

    void on_welcome_clicked();

    //void on_pushButton_clicked();

    void on_Login_clicked();

    void on_Send_clicked();

    void on_SearchButton_clicked();

    void on_b11_clicked();

    void on_b12_clicked();

    void on_b13_clicked();

    void on_b14_clicked();

    void on_b15_clicked();

    void on_b16_clicked();

    void on_b17_clicked();

    void on_b18_clicked();

    void on_b19_clicked();

    void on_b20_clicked();

    void on_b21_clicked();



    void on_b22_clicked();

    void on_b23_clicked();

    void on_b24_clicked();

    void on_b25_clicked();

    void on_b26_clicked();

    void on_b27_clicked();

    void on_b28_clicked();

    void on_b29_clicked();

    void on_b30_clicked();

    void on_b31_clicked();

    void on_b32_clicked();

    void on_b33_clicked();

    void on_b34_clicked();

    void on_b35_clicked();

    void on_b36_clicked();

    void on_b37_clicked();

    void on_b38_clicked();

    void on_b39_clicked();

    void on_b40_clicked();

    void on_b41_clicked();

    void on_b42_clicked();

    void on_b43_clicked();



   // void on_SearchLineEdit_textEdited(const QString &arg1);

    void on_tableView_activated(const QModelIndex &index);
    void on_SearchLineEdit_textEdited(const QString &text);


    void on_b22_2_clicked();

    void runPythonScript(const QString &scriptPath, const QStringList &arguments);

    void on_Capture_clicked() ;
    //void on_Capture_clicked();
    //void imageCaptured(int id, const QImage &preview);





    void on_camicon_linkActivated(const QString );

    void on_video_customContextMenuRequested(const QPoint&);

    void on_EditingFinished();
    //void on_widget_customContextMenuRequested(const QPoint &pos);

    void on_SearchLineEdit_2_textEdited(const QString &arg1);

    void on_AjouterMedicament_clicked() ;


    void on_Envoyer_clicked();





    void on_SearchI_clicked();

    void on_imageCaptured(int id, const QImage& preview) ;


private:
    Ui::MainWindow *ui;
    QStringList clickedButtons;
    void updateTextEdit();
    QString outputResult; // Déclaration de outputResult comme variable membre privée
    QCamera *camera; // Déclaration du membre camera
    QImageCapture *imageCapture = nullptr; // Déclaration du membre imageCapture
    QNetworkAccessManager *networkManager;
    QSqlDatabase db;
    QMap<QString, QString> medsMap; // Map positions to medication names
    QString getMedicationPosition(const QString &medicationName);



    QVector<Drug> selectedDrugs; // Assurez-vous que cette ligne est présente
    QMap<QString, bool> checkedState; // Assurez-vous que cette ligne est présente



};

#endif // MAINWINDOW_H
