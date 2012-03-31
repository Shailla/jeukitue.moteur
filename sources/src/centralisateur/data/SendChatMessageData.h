#ifndef __JKT__SENDCHATMESSAGEDATA_H
#define __JKT__SENDCHATMESSAGEDATA_H

#include <string>
using namespace std;

#include "SDL_net.h"

#include "centralisateur/data/Data.h"

class SendChatMessageData : public Data
{
    string _message;
	string _userName;
public:
    SendChatMessageData(void);
    ~SendChatMessageData(void);

    void setMessage(const string& message);
	void setUserName(const string& userName);

    UDPpacket* toPacket(void);
};

#endif
