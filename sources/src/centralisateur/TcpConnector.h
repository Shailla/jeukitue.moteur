#ifndef __JKT__TCPCONNECTOR_H
#define __JKT__TCPCONNECTOR_H

#include <vector>

#include "SDL_net.h"

#include "centralisateur/data/DownloadFileItem.h"

class ProgressBarView;

namespace jkt
{

class TcpConnector
{
public:
	static std::vector<DownloadFileItem> askDownloadFileList(const int port) noexcept(false);
	static void downloadOneFile(const int port, const long downloadId, ProgressBarView* progressView) noexcept(false);
};

}

#endif
