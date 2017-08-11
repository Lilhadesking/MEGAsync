#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QSslSocket>
#include <QSslKey>
#include <QFile>
#include <QStringList>
#include <QDateTime>
#include <QQueue>

#include <megaapi.h>

class HTTPRequest
{
public:
    HTTPRequest() : contentLength(0), origin(-1) {}
    QString data;
    int contentLength;
    int origin;
};

class HTTPServer: public QTcpServer
{
    Q_OBJECT

    public:
        HTTPServer(mega::MegaApi *megaApi, quint16 port, bool sslEnabled);
#if QT_VERSION >= 0x050000
        void incomingConnection(qintptr socket);
#else
        void incomingConnection(int socket);
#endif
        void pause();
        void resume();

    signals:
        void onLinkReceived(QString link, QString auth);
        void onExternalDownloadRequested(QQueue<mega::MegaNode*> files);
        void onExternalDownloadRequestFinished();
        void onExternalFileUploadRequested(qlonglong targetHandle);
        void onExternalFolderUploadRequested(qlonglong targetHandle);
        void onExternalFolderSyncRequested(qlonglong targetHandle);

    public slots:
        void readClient();
        void discardClient();
        void rejectRequest(QAbstractSocket *socket, QString response = QString::fromUtf8("403 Forbidden"));
        void processRequest(QAbstractSocket *socket, HTTPRequest request);
        void error(QAbstractSocket::SocketError);
        void sslErrors(const QList<QSslError> & errors);
        void peerVerifyError(const QSslError & error);

    private:
        bool disabled;
        bool sslEnabled;
        bool isFirstWebDownloadDone;
        mega::MegaApi *megaApi;
        QMap<QAbstractSocket*, HTTPRequest*> requests;
};

#endif // HTTPSERVER_H
