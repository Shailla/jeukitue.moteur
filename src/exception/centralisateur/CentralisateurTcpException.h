#ifndef __JKT__CENTRALISATEURTCPEXCEPTION_H
#define __JKT__CENTRALISATEURTCPEXCEPTION_H

#include <string>

#include "exception/centralisateur/CentralisateurException.h"

class CentralisateurTcpException : public CentralisateurException
{
	std::string _message;
public:
	CentralisateurTcpException(const std::string& message);
    ~CentralisateurTcpException(void);

	std::string& getMessage();
};

#endif
