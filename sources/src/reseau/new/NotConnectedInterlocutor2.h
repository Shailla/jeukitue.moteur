/*
 * Interlocutor.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef NOTCONNECTEDINTERLOCUTOR_RESEAU_H_
#define NOTCONNECTEDINTERLOCUTOR_RESEAU_H_

#include <string>
#include <queue>

#include "SDL.h"
#include "SDL_net.h"

#include "util/types/Bytes.h"
#include "reseau/new/DataAddress.h"

class NotConnectedInterlocutor2 {
	std::string _name;
	std::queue<DataAddress*> _dataReceived;
	std::queue<DataAddress*> _technicalMessagesReceived;
	std::queue<DataAddress*> _dataToSend;
	std::queue<DataAddress*> _technicalMessagesToSend;
	SDL_mutex* _mutexDataReceived;
	SDL_mutex* _mutexDataToSend;
	SDL_cond* _condDataToSend;
	SDL_cond* _condIntelligence;

public:
	NotConnectedInterlocutor2();
	virtual ~NotConnectedInterlocutor2();

	const std::string& getName() const;
	void setName(const std::string& name);

	void setCondIntelligence(SDL_cond* condIntelligence);

	void pushTechnicalMessageToSend(const IPaddress& address, JktUtils::Bytes* bytes);
	void pushTechnicalMessageToSend(JktUtils::Bytes* bytes);
	DataAddress* popTechnicalMessageToSend();

	void pushTechnicalMessageReceived(const IPaddress& address, JktUtils::Bytes* bytes);
	DataAddress* popTechnicalMessageReceived();

	void pushDataReceived(const IPaddress& address, JktUtils::Bytes* bytes);
	DataAddress* popDataReceived();

	void waitDataToSend(int timeout);
	void pushDataToSend(const IPaddress& address, JktUtils::Bytes* bytes);
	DataAddress* popDataToSend();
};

#endif /* INTERLOCUTOR_RESEAU_H_ */
