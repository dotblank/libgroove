#include <QCryptographicHash>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
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

QString GrooveRequest::generateCacheKey() const
{
    QVariantMap request = buildRequest();
    bool hasKey = false;

    QCryptographicHash crypter(QCryptographicHash::Sha1);
    if (m_method == QLatin1String("getSearchResults")) {
        // hash is:
        // parameters/type
        // parameters/query
        crypter.addData(request["parameters"].toMap()["type"].toByteArray());
        crypter.addData(request["parameters"].toMap()["query"].toByteArray());
        qDebug() << Q_FUNC_INFO << "Search cache key: " << crypter.result().toHex();
        hasKey = true;
    }

    if (!hasKey)
        return QString();

    return crypter.result().toHex();
}

void GrooveRequest::post()
{
    QString cacheKey = generateCacheKey();

    if (cacheKey.length()) {
        qDebug() << Q_FUNC_INFO << "Possibly cached request";
        QString cachePath = QDesktopServices::storageLocation(QDesktopServices::CacheLocation) + "/libgroove/cache/api/" + cacheKey;
        QFile cacheFile(cachePath);
        if (cacheFile.open(QIODevice::ReadOnly)) {
            qDebug() << Q_FUNC_INFO << "Definitely cached request";
            QByteArray response = cacheFile.readAll();
            processData(response);
            return;
        }
    }

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

        QString cacheKey = generateCacheKey();

        // TODO: codeshare with GrooveStream so we don't reinvent caching like this
        if (cacheKey.length()) {
            qDebug() << Q_FUNC_INFO << "Cached a " << response.size() << " byte response with cache key " << cacheKey;
            QString cachePath = QDesktopServices::storageLocation(QDesktopServices::CacheLocation) + "/libgroove/cache/api/";
            QDir dir;
            if (!dir.mkpath(cachePath)) {
            } else {
                cachePath += cacheKey;
                QFile cacheFile(cachePath);
                if (!cacheFile.open(QIODevice::WriteOnly)) {
                    qDebug() << Q_FUNC_INFO << "Cannot open cache file! " << cacheFile.errorString();
                } else {
                    cacheFile.write(response);
                }
            }
        }

        processData(response);
    } else {
        qDebug() << Q_FUNC_INFO << "Not emitting for failed RPC";
        return;
    }

    qDebug() << Q_FUNC_INFO << "Destroying response";
    deleteLater();
}

void GrooveRequest::processData(const QByteArray &response)
{
    bool ok;
    QJson::Parser parser;
    QVariantMap result = parser.parse(response, &ok).toMap();

    if (!ok || result.isEmpty())
        qWarning() << Q_FUNC_INFO << "Couldn't parse response or response was empty: " << response.left(100);

    qDebug() << Q_FUNC_INFO << "Success!";
    emit success(result);
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
