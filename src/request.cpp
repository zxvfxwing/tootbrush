#include "include/request.h"

HttpClient::HttpClient(QObject* parent)
    : QObject(parent)
    , nam(new QNetworkAccessManager(this))
    , ssl_config(new QSslConfiguration(QSslConfiguration::defaultConfiguration()))
    , headers(new QMap<QByteArray, QByteArray>())
    , accepted_timeout(20000)
{
    nam->setNetworkAccessible(QNetworkAccessManager::Accessible);
    setup_ssl();
}

HttpClient::~HttpClient()
{
    delete nam;
    delete ssl_config;
    delete headers;
}

void
HttpClient::setup_ssl()
{
    QList<QByteArray> next_protocols = {
        QByteArray(QSslConfiguration::NextProtocolSpdy3_0)
    };

    ssl_config->setAllowedNextProtocols(next_protocols);
}

QNetworkRequest
HttpClient::build_request(const QUrl& url) const
{
    QNetworkRequest req(url);
    req.setSslConfiguration(*ssl_config);

    QMap<QByteArray, QByteArray>::const_iterator it;
    for(it = headers->constBegin(); it != headers->constEnd(); ++it){
        req.setRawHeader(it.key(), it.value());
    }

    return req;
}

int
HttpClient::process_request(QNetworkReply* reply, QByteArray& answer) const
{
    int status_code = -2;

    QEventLoop* loop = new QEventLoop();
    QTimer* timer = new QTimer();

    connect(reply, &QNetworkReply::finished, loop, &QEventLoop::quit);
    connect(timer, &QTimer::timeout, loop, &QEventLoop::quit);
    timer->start(accepted_timeout);
    loop->exec();

    if( timer->isActive() && reply->isFinished() ){
        timer->stop();
        status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if( reply->error() == QNetworkReply::NetworkError::NoError )
            answer = reply->readAll();
        else {
            answer = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
            if( answer.length() == 0 )
                answer = reply->errorString().toLocal8Bit();
        }
    }
    else {
        reply->abort();
        status_code = -1;
        answer = (QString("tootbrush timeout: request took more than ") + QString::number(accepted_timeout) + QString(" milliseconds.")).toLocal8Bit();
    }

    reply->deleteLater();
    return status_code;
}

void
HttpClient::ping_host(const QString& host) const
{
    nam->connectToHostEncrypted(host, 443, *ssl_config);
}

int
HttpClient::GET(const QUrl& url, QByteArray& answer) const
{
    return process_request(
        nam->get(build_request(url)),
        answer
    );
}

int
HttpClient::POST(const QUrl& url, const QUrlQuery& body, QByteArray& answer) const
{
    return process_request(
        nam->post(
            build_request(url),
            body.toString(QUrl::FullyEncoded).toUtf8()
        ),
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

HttpClient*
HttpClient::set_accepted_timeout(const int& milliseconds)
{
    accepted_timeout = milliseconds;
    return this;
}
