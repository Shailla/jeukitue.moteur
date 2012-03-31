#ifndef __JKT__BINDINGUDPEXCEPTION_H
#define __JKT__BINDINGUDPEXCEPTION_H

#include "exception/centralisateur/CentralisateurException.h"

class BindingUdpException : public CentralisateurException
{
    const char* m_message;
public:
    BindingUdpException(const char* message);
    ~BindingUdpException(void);
};

#endif
