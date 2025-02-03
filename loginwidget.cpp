#include "loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    // Create username input field
    lineEdit_username = new QLineEdit(this);
    lineEdit_username->setPlaceholderText("Username");

    // Create password input field
    lineEdit_password = new QLineEdit(this);
    lineEdit_password->setPlaceholderText("Password");
    lineEdit_password->setEchoMode(QLineEdit::Password);

    // Create login button
    Login = new QPushButton("Login", this);
    //connect(LoginWidget, &QPushButton::clicked, this, &LoginWidget::loginRequested);
    connect(Login, &QPushButton::clicked, this, &LoginWidget::loginRequested);

    // Create layout and add widgets to it
    layout = new QVBoxLayout(this);
    layout->addWidget(lineEdit_username);
    layout->addWidget(lineEdit_password);
    layout->addWidget(Login);

    // Set layout for the widget
    setLayout(layout);
}

QString LoginWidget::getUsername() const
{
    return lineEdit_username->text();
}

QString LoginWidget::getPassword() const
{
    return lineEdit_password->text();
}
