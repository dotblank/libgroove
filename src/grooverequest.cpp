#include <QNetworkRequest>
#include <QNetworkReply>

#include "grooveclient.h"
#include "grooverequest.h"

GrooveRequest::GrooveRequest(QObject *parent) :
    QObject(parent)
{
}

void GrooveRequest::load()
{
    // You forgot to setUrl()?
    Q_ASSERT(!m_url.isEmpty());

    if (m_url.isEmpty()) {
        qWarning("GrooveRequest::load(): ignoring empty URL");
        return;
    }

    QNetworkRequest request = QNetworkRequest(m_url);
    QNetworkReply *reply = GrooveClient::networkManager()->get(request);

    connect(reply, SIGNAL(finished()), SLOT(processRequest()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(processError()));
}

void GrooveRequest::processRequest()
{
    loaded(qobject_cast<QNetworkReply *>(sender())->readAll());
    emit finished();
}

void GrooveRequest::handleError()
{
    emit finished();
}

void GrooveRequest::setUrl(const QUrl &requestUrl)
{
    m_url = requestUrl;
}
