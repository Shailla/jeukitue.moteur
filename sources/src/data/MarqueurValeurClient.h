/*
 * MarqueurValeurClient.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef MARQUEURVALEURCLIENT_H_
#define MARQUEURVALEURCLIENT_H_

#include "data/MarqueurClient.h"
#include "data/Branche.h"

class MarqueurValeurClient : public MarqueurClient {
	Valeur* _valeur;
public:
	MarqueurValeurClient(Valeur* valeur);
	virtual ~MarqueurValeurClient();
};

#endif /* MARQUEURVALEURCLIENT_H_ */
