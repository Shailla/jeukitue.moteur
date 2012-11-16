#include <string>

#include "centralisateur/UdpController.h"
#include "centralisateur/TcpConnector.h"
#include "centralisateur/data/SignalementData.h"
#include "centralisateur/data/DownloadFileItem.h"

#include "centralisateur/Centralisateur.h"

Centralisateur::Centralisateur(UdpController* udpController)
{
    _udpController = udpController;
    _portLocal = 0;
}

Centralisateur::~Centralisateur(void)
{
    delete _udpController;
}

void Centralisateur::connecter(char* userName, char* ipCentralisateur, int portCentralisateur)
{
    const int _portLocal = 39455;
    _userName = userName;
    _udpController->connect(userName, _portLocal, ipCentralisateur, portCentralisateur);
}

void Centralisateur::sendSignalement()
{
    _udpController->sendSignalement(_userName);
}

void Centralisateur::sendMessageChat(const std::string& message, const std::string& userName)
{
    _udpController->sendMessageChat(message, userName);
}

vector<DownloadFileItem> Centralisateur::askDownloadFileList(const int port) throw(CentralisateurTcpException)
{
    return JktNet::TcpConnector::askDownloadFileList(port);
}

void Centralisateur::downloadOneFile(const int port, const long downloadId, ProgressBarView* progressView) throw(CentralisateurTcpException)
{
    JktNet::TcpConnector::downloadOneFile(port, downloadId, progressView);
}
