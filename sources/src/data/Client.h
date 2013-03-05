/*
 * Client.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <vector>

using namespace std;

#include "data/MarqueurClient.h"

class Client {
	vector<MarqueurClient> _marqueurs;
public:
	Client();
	virtual ~Client();

	vector<MarqueurClient>& getMarqueurs();

	void addMarqueur(MarqueurClient& marqueur);
};

#endif /* CLIENT_H_ */
