/*
 * EchoCommand.h
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#ifndef ECHOCOMMAND_H_
#define ECHOCOMMAND_H_

#include <string>

#include "script/commandes/Commande.h"

class EchoCommande : public Commande {
	void executeIt(std::string ligne, bool userOutput);

public:
	EchoCommande (CommandeInterpreter* interpreter);

	std::	string getShortHelp() const;
	std::string getHelp() const;
};

#endif /* ECHOCOMMAND_H_ */
