/*
 * Interlocutor.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef INTERLOCUTOR_RESEAU_H_
#define INTERLOCUTOR_RESEAU_H_

#include <string>
#include <queue>

#include "SDL.h"
#include "SDL_net.h"

#include "util/types/Bytes.h"

class Interlocutor2 {
	std::string _name;

	std::queue<jkt::Bytes*> _dataReceived;
	std::queue<jkt::Bytes*> _technicalMessagesReceived;
	std::queue<jkt::Bytes*> _dataToSend;
	std::queue<jkt::Bytes*> _technicalMessagesToSend;
	SDL_mutex* _mutexDataReceived;
	SDL_mutex* _mutexDataToSend;
	SDL_cond* _condDataToSend;
	SDL_cond* _condIntelligence;

public:
	Interlocutor2(SDL_cond* condDataToSend, SDL_mutex* mutexDataToSend);
	virtual ~Interlocutor2();

	const std::string& getName() const;
	void setName(const std::string& name);

	void setCondIntelligence(SDL_cond* condIntelligence);

	// Technical messages
	void pushTechnicalMessageToSend(jkt::Bytes* bytes);
	jkt::Bytes* popTechnicalMessageToSend();

	void pushTechnicalMessageReceived(jkt::Bytes* bytes);
	jkt::Bytes* popTechnicalMessageReceived();

	int countTechnicalMessagesReceived(void);
	int countTechnicalMessagesToSend(void);

	// Data messages
	void pushDataReceived(jkt::Bytes* bytes);
	jkt::Bytes* popDataReceived();

	void waitDataToSend(int timeout);
	void pushDataToSend(jkt::Bytes* bytes);
	jkt::Bytes* popDataToSend();

	int countDataReceived(void);
	int countDataToSend(void);
};

#endif /* INTERLOCUTOR_RESEAU_H_ */
