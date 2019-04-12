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

bool
HttpClient::process_request(QNetworkReply* reply, QByteArray& answer) const
{
    while( !reply->isFinished() )
        qApp->processEvents();

    reply->deleteLater();

    if( reply->error() != QNetworkReply::NoError ){
        answer = "Failed";
        return false;
    }

    answer = reply->readAll();
    return true;
}

void
HttpClient::ping_host(const QString& host) const
{
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();

    QList<QByteArray> next_protocols = {
        QByteArray(QSslConfiguration::NextProtocolSpdy3_0)
    };

    config.setAllowedNextProtocols(next_protocols);
    nam->connectToHostEncrypted(host, 443, config);
}

bool
HttpClient::GET(const QUrl& url, QByteArray& answer) const
{
    return process_request(
        nam->get(build_request(url)),
        answer
    );
}

bool
HttpClient::POST(const QUrl& url, const QUrlQuery& body, QByteArray& answer) const
{
    return process_request(
        nam->post(build_request(url),
        body.toString(QUrl::FullyEncoded).toUtf8()),
        answer
    );
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
