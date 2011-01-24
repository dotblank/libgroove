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
QString const GrooveRequest::REVISION     = "20100831.19";

GrooveRequest::GrooveRequest(GrooveClient *client, QString service)
    : m_request(QUrl(service))
    , m_client(client)
{
    m_headers.insert("client", "htmlshark");
    m_headers.insert("clientRevision", GrooveRequest::REVISION);
}

QVariantMap GrooveRequest::buildRequest() const
{
    QVariantMap jlist;
    jlist.insert("parameters", m_parameters);
    jlist.insert("header", m_headers);
    jlist.insert("method", m_method);
    return jlist;
}

void GrooveRequest::post(QObject const *receiver, char const *slot)
{
    m_request.setHeader(m_request.ContentTypeHeader, "application/json");

    QJson::Serializer serializer;

    qDebug() << Q_FUNC_INFO << "Posting to: " << m_request.url();
    qDebug() << Q_FUNC_INFO << serializer.serialize(buildRequest());

    QNetworkReply *reply = m_client->networkManager()->post(m_request, serializer.serialize(buildRequest()));
    receiver->connect(reply, SIGNAL (finished ()), slot, Qt::AutoConnection);
}

void GrooveRequest::get(QObject const *receiver, char const *slot)
{
    m_request.setHeader(m_request.ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = m_client->networkManager()->get(m_request);
    receiver->connect(reply, SIGNAL (finished ()), slot, Qt::AutoConnection);
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
