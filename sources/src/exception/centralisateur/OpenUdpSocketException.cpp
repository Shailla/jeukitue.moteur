#include "exception/centralisateur/OpenUdpSocketException.h"

OpenUdpSocketException::OpenUdpSocketException(const int port, const char* message)
{
    m_port = port;
    m_message = message;
}

OpenUdpSocketException::~OpenUdpSocketException(void)
{
}
