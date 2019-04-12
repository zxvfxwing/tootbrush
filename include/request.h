#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>

#include <QApplication>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QSslConfiguration>

class HttpClient : public QObject {
    Q_OBJECT

private:
    QNetworkAccessManager* nam;
    QMap<QByteArray, QByteArray>* headers;
    QNetworkRequest build_request(const QUrl&) const;

    bool process_request(QNetworkReply* reply, QByteArray& answer) const;

public:
    explicit HttpClient(QObject* parent=nullptr);
    virtual ~HttpClient();

    void ping_host(const QString& host) const;

    bool GET(const QUrl& url, QByteArray& answer) const;
    bool POST(const QUrl& url, const QUrlQuery& body, QByteArray& answer) const;

    HttpClient* set_header(const QByteArray& header, const QByteArray& value);
    HttpClient* remove_header(const QByteArray& header);
    HttpClient* remove_all_headers();
};

#endif // REQUEST_H
