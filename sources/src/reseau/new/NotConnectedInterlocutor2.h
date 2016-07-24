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
#include "reseau/new/Interlocutor2.h"

class NotConnectedInterlocutor2 {
	std::string _name;
	std::queue<DataAddress*> _dataReceived;
	std::queue<DataAddress*> _technicalMessagesReceived;
	std::queue<DataAddress*> _dataToSend;
	std::queue<DataAddress*> _technicalMessagesToSend;
	std::queue<Interlocutor2*> _newInterlocutors;

	SDL_mutex* _mutexDataReceived;
	SDL_mutex* _mutexDataToSend;
	SDL_cond* _condDataToSend;
	SDL_cond* _condIntelligence;
	SDL_mutex* _mutexNewInterlocutors;

public:
	NotConnectedInterlocutor2(SDL_cond* condDataToSend, SDL_mutex* mutexDataToSend);
	virtual ~NotConnectedInterlocutor2();

	const std::string& getName() const;
	void setName(const std::string& name);

	void setCondIntelligence(SDL_cond* condIntelligence);

	void pushTechnicalMessageToSend(const IPaddress& address, jkt::Bytes* bytes);
	void pushTechnicalMessageToSend(jkt::Bytes* bytes);
	DataAddress* popTechnicalMessageToSend();

	void pushTechnicalMessageReceived(const IPaddress& address, jkt::Bytes* bytes);
	DataAddress* popTechnicalMessageReceived();

	void pushDataReceived(const IPaddress& address, jkt::Bytes* bytes);
	DataAddress* popDataReceived();

	void waitDataToSend(int timeout);
	void pushDataToSend(const IPaddress& address, jkt::Bytes* bytes);
	DataAddress* popDataToSend();


	/** Push the interlocutor of a client who has just been connected to the server in the list of the new server's clients. */
	void pushNewInterlocutor(Interlocutor2* newInterlocutor);

	/** Pop the interlocutor of a client who has been connected to the server. */
	Interlocutor2* popNewInterlocutor();
};

#endif /* INTERLOCUTOR_RESEAU_H_ */
