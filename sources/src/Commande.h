/*
 * Commande.h
 *
 *  Created on: 6 mars 2013
 *      Author: vgdj7997
 */

#ifndef COMMANDE_H_
#define COMMANDE_H_

#include <string>

class Commande {
public:
	Commande();
	virtual ~Commande();

	void execute(std::string ligne, bool userOutput);
};

#endif /* COMMANDE_H_ */
