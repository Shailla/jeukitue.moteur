#ifndef __JKT__TCPCONNECTOR_H
#define __JKT__TCPCONNECTOR_H

#include <string>
#include <vector>
using namespace std;

#include "SDL_net.h"

#include "exception/centralisateur/CentralisateurTcpException.h"
#include "centralisateur/data/DownloadFileItem.h"
#include "menu/ProgressBarView.h"

namespace JktNet
{

class TcpConnector
{
public:
	static vector<DownloadFileItem> askDownloadFileList(const int port) throw(CentralisateurTcpException);
	static void downloadOneFile(const int port, const long downloadId, ProgressBarView* progressView) throw(CentralisateurTcpException);
};

}

#endif