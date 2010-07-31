#ifndef GROOVEREQUEST_H
#define GROOVEREQUEST_H

#include <QObject>
#include <QUrl>

#include "libgroove_global.h"

class LIBGROOVESHARED_EXPORT GrooveRequest : public QObject
{
    Q_OBJECT
public:
    explicit GrooveRequest(QObject *parent = 0);

public slots:
    void load();

signals:
    /*!
        Signals the request has completed and is ready for disposal.
    */
    void finished();

protected:
    /*!
        Called when data \a dataRecieved has been returned from GrooveShark ready for processing.
    */
    virtual void loaded(const QString &dataRecieved) = 0;

    /*!
        Sets the URL for a given request. Must be called from the constructor of a derived class.
    */
    void setUrl(const QUrl &requestUrl);

private slots:
    void processRequest();
    void handleError();

private:
    QUrl m_url;
};

#endif // GROOVEREQUEST_H
