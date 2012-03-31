#include "exception/centralisateur/BindingUdpException.h"

BindingUdpException::BindingUdpException(const char* message)
{
    m_message = message;
}

BindingUdpException::~BindingUdpException(void)
{
}
