/*
 * MarqueurValeurClient.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef MARQUEURVALEURCLIENT_H_
#define MARQUEURVALEURCLIENT_H_

class MarqueurValeurClient : MarqueurClient {
	Valeur& _valeur;
public:
	MarqueurValeurClient(const Valeur& valeur);
	virtual ~MarqueurValeurClient();
};

#endif /* MARQUEURVALEURCLIENT_H_ */
