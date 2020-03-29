#ifndef __JKT__CENRALISATEUR_H
#define __JKT__CENRALISATEUR_H

#include <string>
#include <vector>

#include "centralisateur/data/DownloadFileItem.h"

class UdpController;
class ProgressBarView;

class Centralisateur {
    int _portLocal;
    UdpController* _udpController;
    std::string _userName;
public:
    Centralisateur(UdpController* udpController);
    ~Centralisateur(void);

    void connecter(char* userName, char* adresseIp, int port);
    void sendSignalement(void);
	void sendMessageChat(const std::string& message, const std::string& userName);

	std::vector<DownloadFileItem> askDownloadFileList(const int port) noexcept(false);
	void downloadOneFile(const int port, const long downloadId, ProgressBarView* progressView) noexcept(false);
};

#endif
