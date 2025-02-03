#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);
    QString getUsername() const;
    QString getPassword() const;

signals:
    void loginRequested();


private:
    QLineEdit *lineEdit_username;
    QLineEdit *lineEdit_password;
    QPushButton *Login;
    QVBoxLayout *layout;
};

#endif // LOGINWIDGET_H
