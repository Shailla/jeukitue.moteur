/*
 * Client.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <map>

#include "data/MarqueurBrancheClient.h"
#include "data/MarqueurValeurClient.h"

class Client {
	std::string _debugName;
	std::map<Branche*, MarqueurBrancheClient*> _marqueursBranche;
	std::map<Valeur*, MarqueurValeurClient*> _marqueursValeur;
public:
	Client(const string& debugName);
	virtual ~Client();

	MarqueurBrancheClient* getMarqueurBranche(Branche* branche);
	MarqueurValeurClient* getMarqueurValeur(Valeur* valeur);
	std::map<Branche*, MarqueurBrancheClient*>& getMarqueursBranche();
	std::map<Valeur*, MarqueurValeurClient*>& getMarqueursValeur();
	std::string getDebugName();
	void addMarqueur(MarqueurBrancheClient* marqueur);
	void addMarqueur(MarqueurValeurClient* marqueur);
};

#endif /* CLIENT_H_ */
