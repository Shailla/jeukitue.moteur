/*
 * MarqueurBrancheClient.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef MARQUEURBRANCHECLIENT_H_
#define MARQUEURBRANCHECLIENT_H_

#include "data/MarqueurClient.h"
#include "data/Branche.h"

class MarqueurBrancheClient : public MarqueurClient {
	Branche* _branche;
public:
	MarqueurBrancheClient(Branche* branche);
	virtual ~MarqueurBrancheClient();

	Branche* getBranche();
};

#endif /* MARQUEURBRANCHECLIENT_H_ */
