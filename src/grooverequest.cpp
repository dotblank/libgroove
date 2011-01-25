#include <QObject>
#include <QNetworkReply>

// QJson
#include <qjson/parser.h>
#include <qjson/serializer.h>

// Us
#include "grooverequest.h"
#include "grooveclient.h"

QString const GrooveRequest::API_URL      = "https://cowbell.grooveshark.com/";
QString const GrooveRequest::ART_BASE_URL = "http://beta.grooveshark.com/static/amazonart/m";
QString const GrooveRequest::LOGIN_URL    = "http://listen.grooveshark.com/";
QString const GrooveRequest::REVISION     = "20101012.37";

GrooveRequest::GrooveRequest(GrooveClient *client, QString service)
    : m_request(QUrl(service))
    , m_client(client)
{
    qDebug() << Q_FUNC_INFO << "Creating request to " << service;
    m_headers.insert("client", "htmlshark");
    m_headers.insert("clientRevision", GrooveRequest::REVISION);

    QVariantMap country;
    country.insert("CC1","0");
    country.insert("CC3","0");
    country.insert("ID","223");
    country.insert("CC2","0");
    country.insert("CC4","1073741824");

    m_headers.insertMulti("country", country);
    m_parameters.insertMulti("country", country);
}

QVariantMap GrooveRequest::buildRequest() const
{
    QVariantMap jlist;
    jlist.insert("parameters", m_parameters);
    jlist.insert("header", m_headers);
    jlist.insert("method", m_method);
    return jlist;
}

void GrooveRequest::post()
{
    m_request.setHeader(m_request.ContentTypeHeader, "application/json");

    QJson::Serializer serializer;

    qDebug() << Q_FUNC_INFO << "Posting to: " << m_request.url();
    qDebug() << Q_FUNC_INFO << serializer.serialize(buildRequest());

    QNetworkReply *reply = m_client->networkManager()->post(m_request, serializer.serialize(buildRequest()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SIGNAL(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), SLOT(onFinished()));
}

void GrooveRequest::get()
{
    m_request.setHeader(m_request.ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = m_client->networkManager()->get(m_request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SIGNAL(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), SLOT(onFinished()));
}

void GrooveRequest::onFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (GROOVE_VERIFY(reply, "search returned without a QNetworkReply")) return;

    if (!reply->error()) {
        QByteArray response = reply->readAll();
        bool ok;
        QJson::Parser parser;
        QVariantMap result = parser.parse(response, &ok).toMap();

        if (!ok)
            qWarning() << Q_FUNC_INFO << "Couldn't parse result of response: " << response.left(100);

        emit success(result);
    } else {
        qDebug() << Q_FUNC_INFO << "Not emitting for failed RPC";
        return;
    }

    qDebug() << Q_FUNC_INFO << "Destroying response";
    deleteLater();
}

void GrooveRequest::setHeader(const QString &header, const QVariant &value)
{
    m_headers[header] = value;
}

void GrooveRequest::setParameter(const QString &parameter, const QVariant &value)
{
    m_parameters[parameter] = value;
}

void GrooveRequest::setMethod(const QString &method)
{
    m_method = method;
}
