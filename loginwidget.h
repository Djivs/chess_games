#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>

#include <QPair>

class LoginWidget : public QWidget {
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

    QPair<QString, QString> getUserInputs();

signals:
    void authorize();

private:
    QPushButton *loginButton;
    QLineEdit *login;
    QLineEdit *password;

    QGridLayout *layout;

signals:

};

#endif // LOGINWIDGET_H
