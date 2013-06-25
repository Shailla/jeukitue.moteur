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
public:
	class DataAddress {
		IPaddress _address;
		JktUtils::Bytes* _bytes;

	public:
		DataAddress(char* data, int size) {
			_bytes = new JktUtils::Bytes(data, size);
		}

		DataAddress(JktUtils::Bytes* bytes) {
			_bytes = bytes;
		}

		DataAddress(const IPaddress& address, char* data, int size) {
			_address = address;
			_bytes = new JktUtils::Bytes(data, size);
		}

		DataAddress(const IPaddress& address, JktUtils::Bytes* bytes) {
			_address = address;
			_bytes = bytes;
		}

		~DataAddress() {
			delete _bytes;
		}

		const IPaddress& getAddress() const {
			return _address;
		}

		JktUtils::Bytes* getBytes() {
			return _bytes;
		}
	};

private:
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
	Interlocutor2();
	virtual ~Interlocutor2();

	const std::string& getName() const;
	void setName(const std::string& name);

	void setCondIntelligence(SDL_cond* condIntelligence);

	void pushTechnicalMessageReceived(const IPaddress& address, JktUtils::Bytes* bytes);
	DataAddress* popTechnicalMessageReceived();

	void pushTechnicalMessageToSend(JktUtils::Bytes* bytes);
	void pushTechnicalMessageToSend(const IPaddress& address, JktUtils::Bytes* bytes);
	DataAddress* popTechnicalMessageToSend();

	void pushDataReceived(const IPaddress& address, JktUtils::Bytes* bytes);
	DataAddress* popDataReceived();

	void waitDataToSend(int timeout);
	void pushDataToSend(const IPaddress& address, JktUtils::Bytes* bytes);
	DataAddress* popDataToSend();
};

#endif /* INTERLOCUTOR_RESEAU_H_ */
