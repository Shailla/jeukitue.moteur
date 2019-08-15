/*
 * EchoCommand.h
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#ifndef DATACOMMAND_H_
#define DATACOMMAND_H_

#include <string>

#include "script/commandes/Commande.h"

class DataCommande : public Commande {
	void executeIt(std::string ligne, bool userOutput) noexcept(false);

public:
	DataCommande (CommandeInterpreter* interpreter);

	std::string getShortHelp() const;
	std::string getHelp() const;
};

#endif /* ECHOCOMMAND_H_ */
