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

class Interlocutor2 {
	std::string _name;
	std::queue<char*> _dataReceived;
	std::queue<char*> _technicalMessagesReceived;
	std::queue<char*> _dataToSend;
	std::queue<char*> _technicalMessagesToSend;
	SDL_mutex* _mutexDataReceived;
	SDL_mutex* _mutexDataToSend;
	SDL_cond* _condDataToSend;
	SDL_cond* _condIntelligence;

public:
	void setCondIntelligence(SDL_cond* condIntelligence);

	void addTechnicalMessageReceived(char* msg);
	char* getTechnicalMessageReceived();

	void addTechnicalMessageToSend(char* msg);
	char* getTechnicalMessageToSend();

	Interlocutor2(const std::string& name);
	virtual ~Interlocutor2();

	const std::string& getName() const;

	void addDataReceived(char* data);
	char* getDataReceived();

	void waitDataToSend(int timeout);
	void addDataToSend(char* data);
	char* getDataToSend();
};

#endif /* INTERLOCUTOR_RESEAU_H_ */
