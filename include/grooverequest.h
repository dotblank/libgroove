#ifndef GROOVEREQUEST_H
#define GROOVEREQUEST_H

// NOTE:
// this class is not exported API, use it at your own risk

// Qt
#include <QNetworkRequest>
#include <QVariantMap>

// Us
#include "libgroove_global.h"
#include "grooveclient.h"

class GrooveRequest
{
public:
    static QString const API_URL;
    static QString const ART_BASE_URL;
    static QString const LOGIN_URL;
    static QString const REVISION;

    static QString more (QString const &method)
    {
      return API_URL + "more.php?" + method;
    }

    static QString service ()
    {
      return API_URL + "service.php";
    }

    static QString stream (QString const &ip)
    {
      return "http://" + ip + "/stream.php";
    }

    explicit GrooveRequest(GrooveClient *client, QString service);

    QVariantMap buildRequest() const;

    void get(QObject const *receiver, char const *slot);
    void post(QObject const *receiver, char const *slot);

    void setHeader(const QString &header, const QVariant &value);
    void setParameter(const QString &parameter, const QVariant &value);
    void setMethod(const QString &method);

    QNetworkRequest m_request;
    GrooveClient *m_client;
    QVariantMap m_headers;
    QVariantMap m_parameters;
    QString m_method;
};


#endif // GROOVEREQUEST_H
