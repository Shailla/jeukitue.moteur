#include "exception/centralisateur/CentralisateurTcpException.h"

CentralisateurTcpException::CentralisateurTcpException(const std::string& message)
{
    _message = message;
}

CentralisateurTcpException::~CentralisateurTcpException(void)
{
}

std::string& CentralisateurTcpException::getMessage()
{
	return _message;
}
