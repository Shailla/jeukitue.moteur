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
#include <sstream>

#include "data/Donnee.h"
#include "data/MarqueurClient.h"
#include "data/communication/message/Changement.h"

class Client {
	std::string _debugName;
	std::map<Donnee*, MarqueurClient*> _marqueurs;

	string* _dataTest;
public:
	Client(const string& debugName);
	virtual ~Client();

	MarqueurClient* getMarqueur(Donnee* donnee);
	std::map<Donnee*, MarqueurClient*>& getMarqueurs();
	std::string& getDebugName();
	void addMarqueur(Donnee* donnee, int donneeTmpId);
	void collecteChangements(std::vector<Changement*>& changements);

	void sendData(std::ostringstream& out);
	std::string receiveData(void);
};

#endif /* CLIENT_H_ */
