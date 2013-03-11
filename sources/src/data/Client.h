/*
 * Client.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <map>
#include <vector>

#include "data/Donnee.h"
#include "data/MarqueurClient.h"
#include "data/Changement.h"

class Client {
	std::string _debugName;
	std::map<Donnee*, MarqueurClient*> _marqueurs;
public:
	Client(const string& debugName);
	virtual ~Client();

	MarqueurClient* getMarqueur(Donnee* donnee);
	std::map<Donnee*, MarqueurClient*>& getMarqueurs();
	std::string& getDebugName();
	void addMarqueur(Donnee* donnee, int donneeTmpId, bool isUpToDate);
	void collecteChangements(std::vector<Changement*>& changements);

	void sendData(char* data);
	void receiveData(char* data);
};

#endif /* CLIENT_H_ */
