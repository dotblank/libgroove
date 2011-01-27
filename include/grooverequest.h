#ifndef GROOVEREQUEST_H
#define GROOVEREQUEST_H

// NOTE:
// this class is not exported API, use it at your own risk

// Qt
#include <QNetworkReply>
#include <QVariantMap>
#include <QObject>

// Us
#include "libgroove_global.h"
#include "grooveclient.h"

class GrooveRequest : public QObject
{
    Q_OBJECT
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
    QString generateCacheKey() const;
    bool processData(const QByteArray &response);

    void get();
    void post();

    void setHeader(const QString &header, const QVariant &value);
    void setParameter(const QString &parameter, const QVariant &value);
    void setMethod(const QString &method);

    QString errorString() const;

signals:
    /*!
      Emitted when a request has succeeded, passes all of the \a data
      read from the RPC request, parsed from JSON.
    */
    void success(const QVariantMap &data);

    /*!
      Emitted when an RPC request fails, along with the \a errorCode
      explaining the failure.
    */
    void error(QNetworkReply::NetworkError errorCode);

private slots:
    void onFinished();


private:

    QNetworkRequest m_request;
    GrooveClient *m_client;
    QVariantMap m_headers;
    QVariantMap m_parameters;
    QString m_method;
};


#endif // GROOVEREQUEST_H
