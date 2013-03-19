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
#include "data/MarqueurDistant.h"
#include "data/communication/message/Changement.h"

class Distant {
	std::string _debugName;
	std::map<Donnee*, MarqueurDistant*> _marqueurs;

	string* _dataTest;
public:
	Distant(const string& debugName);
	virtual ~Distant();

	MarqueurDistant* getMarqueur(Donnee* donnee);
	std::map<Donnee*, MarqueurDistant*>& getMarqueurs();
	std::string& getDebugName();
	MarqueurDistant* addMarqueur(Donnee* donnee, int donneeTmpId);
	void collecteChangements(std::vector<Changement*>& changements);

	void sendData(std::ostringstream& out);
	std::string* getDataToSend();
};

#endif /* CLIENT_H_ */
