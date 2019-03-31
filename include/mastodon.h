#ifndef MASTODON_APP_H
#define MASTODON_APP_H

#include <QUrl>
#include <QString>
#include <QByteArray>

namespace API {
    namespace OAUTH {
        const QString Authorize             = "/oauth/authorize";
        const QString Token                 = "/oauth/token";
    }
    namespace V1 {
        const QString Apps                  = "/api/v1/apps";
        const QString Verify_Credentials    = "/api/v1/verify_credentials";
    }

    QUrl build_url(const QString& host, const QString& api)
    {
        return QUrl(host + api);
    }
}

class MastodonApp {
private:
    QString name;
    QString website;
    QString client_id;
    QString client_secret;

public:
    explicit MastodonApp(QByteArray array);
    virtual ~MastodonApp();
};



#endif // MASTODON_APP_H
