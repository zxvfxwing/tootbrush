#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QMainWindow>
#include <QMenuBar>
#include <QLineEdit>
#include <QApplication>
#include <QPushButton>
#include <QRegExpValidator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget* main;
    QLineEdit* instance_url;
    QMenuBar*  menu_bar;

    QMenu* menu_file;
    QAction* action_exit;

    void build_main_layout(QBoxLayout*);
    void create_menu();

    void http_request();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
