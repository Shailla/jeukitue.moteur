#ifndef __JKT__CENRALISATEUR_H
#define __JKT__CENRALISATEUR_H

#include <string>
#include <vector>

using namespace std;

#include "centralisateur/data/DownloadFileItem.h"
#include "exception/centralisateur/CentralisateurTcpException.h"

#include "UdpController.h"

class Centralisateur
{
    int _portLocal;
    UdpController* _udpController;
    string _userName;
public:
    Centralisateur(UdpController* udpController);
    ~Centralisateur(void);

    void connecter(char* userName, char* adresseIp, int port);
    void sendSignalement(void);
	void sendMessageChat(const string& message, const string& userName);

	vector<DownloadFileItem> askDownloadFileList(const int port) throw(CentralisateurTcpException);
	void downloadOneFile(const int port, const long fileId, int* progress, char* currentOperationMessage) throw(CentralisateurTcpException);
};

#endif
