#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>

#include <QApplication>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

class HttpClient : public QObject {
    Q_OBJECT

private:
    QNetworkAccessManager* nam;
    QMap<QByteArray, QByteArray>* headers;

    const QByteArray process_request(QNetworkReply*) const;
    QNetworkRequest build_request(const QUrl&) const;

public:
    explicit HttpClient(QObject* parent=nullptr);
    virtual ~HttpClient();

    const QByteArray GET(const QUrl&);
    const QByteArray POST(const QUrl&, const QUrlQuery&);

    HttpClient* set_header(const QByteArray& header, const QByteArray& value);
    HttpClient* remove_header(const QByteArray& header);
    HttpClient* remove_all_headers();
};

#endif // REQUEST_H
