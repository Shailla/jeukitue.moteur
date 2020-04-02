#ifndef __JKT__SENDCHATMESSAGEDATA_H
#define __JKT__SENDCHATMESSAGEDATA_H

#include <string>

#include <SDL_net.h>

#include "centralisateur/data/Data.h"

class SendChatMessageData : public Data {
	std::string _message;
	std::string _userName;
public:
    SendChatMessageData(void);
    ~SendChatMessageData(void);

    void setMessage(const std::string& message);
	void setUserName(const std::string& userName);

    UDPpacket* toPacket(void);
};

#endif
