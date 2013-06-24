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

class Interlocutor2 {
public:
	class DataAddress {
		IPaddress _address;
		char* _data;

	public:
		DataAddress(const IPaddress& address, char* data) {
			_address = address;
			_data = data;
		}

		~DataAddress() {
			delete[] _data;
		}

		const IPaddress& getAddress() const {
			return _address;
		}

		char* getData() {
			return _data;
		}
	};

private:
	std::string _name;
	std::queue<char*> _dataReceived;
	std::queue<DataAddress*> _technicalMessagesReceived;
	std::queue<char*> _dataToSend;
	std::queue<char*> _technicalMessagesToSend;
	SDL_mutex* _mutexDataReceived;
	SDL_mutex* _mutexDataToSend;
	SDL_cond* _condDataToSend;
	SDL_cond* _condIntelligence;

public:
	Interlocutor2();
	virtual ~Interlocutor2();

	const std::string& getName() const;
	void setName(const std::string& name);

	void setCondIntelligence(SDL_cond* condIntelligence);

	void pushTechnicalMessageReceived(const IPaddress& address, char* msg);
	DataAddress* popTechnicalMessageReceived();

	void pushTechnicalMessageToSend(char* msg);
	char* popTechnicalMessageToSend();

	void pushDataReceived(char* data);
	char* popDataReceived();

	void waitDataToSend(int timeout);
	void pushDataToSend(char* data);
	char* popDataToSend();
};

#endif /* INTERLOCUTOR_RESEAU_H_ */
