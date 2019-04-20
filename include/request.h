#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>

#include <QApplication>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QSslConfiguration>
#include <QEventLoop>
#include <QTimer>

class HttpClient : public QObject {
    Q_OBJECT

private:
    QNetworkAccessManager* nam;
    QSslConfiguration* ssl_config;
    QMap<QByteArray, QByteArray>* headers;
    int accepted_timeout;

    void setup_ssl();

    QNetworkRequest build_request(const QUrl&) const;
    int process_request(QNetworkReply* reply, QByteArray& answer) const;

public:
    explicit HttpClient(QObject* parent=nullptr);
    virtual ~HttpClient();

    void ping_host(const QString& host) const;

    int GET(const QUrl& url, QByteArray& answer) const;
    int POST(const QUrl& url, const QUrlQuery& body, QByteArray& answer) const;

    HttpClient* set_header(const QByteArray& header, const QByteArray& value);
    HttpClient* remove_header(const QByteArray& header);
    HttpClient* remove_all_headers();
    HttpClient* set_accepted_timeout(const int& milliseconds);
};

#endif // REQUEST_H
