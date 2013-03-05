/*
 * MarqueurBrancheClient.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef MARQUEURBRANCHECLIENT_H_
#define MARQUEURBRANCHECLIENT_H_

class MarqueurBrancheClient : MarqueurClient {
	Branche& _branche;
public:
	MarqueurBrancheClient(const Branche& branche);
	virtual ~MarqueurBrancheClient();
};

#endif /* MARQUEURBRANCHECLIENT_H_ */
