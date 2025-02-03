#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QCamera>
#include <QImageCapture>
#include <QVBoxLayout>
#include <QLabel>
#include<QVideoWidget>
#include <QMediaPlayer>



class CamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CamWidget(QWidget *parent = nullptr);
    ~CamWidget();

private slots:
    //void captureImage();

private:

};

#endif // CAMWIDGET_H
