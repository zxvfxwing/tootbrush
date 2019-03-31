#include "include/request.h"

HttpClient::HttpClient(QObject* parent)
    : QObject(parent)
    , nam(new QNetworkAccessManager(this))
    , headers(new QMap<QByteArray, QByteArray>())
{
    nam->setNetworkAccessible(QNetworkAccessManager::Accessible);
}

HttpClient::~HttpClient()
{
    delete nam;
    delete headers;
}

const QByteArray
HttpClient::process_request(QNetworkReply* r) const
{
    while(!r->isFinished()){
        qApp->processEvents();
    }

    r->deleteLater();
    return r->readAll();
}

QNetworkRequest
HttpClient::build_request(const QUrl& url) const
{
    QNetworkRequest req(url);

    QMap<QByteArray, QByteArray>::const_iterator it;
    for(it = headers->constBegin(); it != headers->constEnd(); ++it){
        req.setRawHeader(it.key(), it.value());
    }

    return req;
}

const QByteArray
HttpClient::GET(const QUrl& url)
{
    return process_request(nam->get(build_request(url)));
}

const QByteArray
HttpClient::POST(const QUrl& url, const QUrlQuery& query)
{
    return process_request(
        nam->post(
            build_request(url),
            query.toString(QUrl::FullyEncoded).toUtf8()
    ));
}

HttpClient*
HttpClient::set_header(const QByteArray& header, const QByteArray& value)
{
    headers->insert(header, value);
    return this;
}

HttpClient*
HttpClient::remove_header(const QByteArray& header)
{
    headers->remove(header);
    return this;
}

HttpClient*
HttpClient::remove_all_headers()
{
    headers->clear();
    return this;
}
