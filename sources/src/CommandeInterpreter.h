/*
 * CommandeInterpreter.h
 *
 *  Created on: 6 mars 2013
 *      Author: vgdj7997
 */

#ifndef COMMANDEINTERPRETER_H_
#define COMMANDEINTERPRETER_H_

#include <map>
#include <string>

#include "Commande.h"

class CommandeInterpreter {
	std::map<std::string, Commande> _commandes;
public:
	CommandeInterpreter();
	virtual ~CommandeInterpreter();

	void interpreteCommande(std::string commande, bool userOutput);
};

#endif /* COMMANDEINTERPRETER_H_ */
