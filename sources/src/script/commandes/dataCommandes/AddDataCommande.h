/*
 * EchoCommand.h
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#ifndef ADDDATACOMMAND_H_
#define ADDDATACOMMAND_H_

#include <string>

#include "script/commandes/Commande.h"

#include "script/commandes/dataCommandes/AddDataCommande.h"

class AddDataCommande : public Commande {
	void executeIt(std::string ligne, bool userOutput) noexcept(false);

public:
	AddDataCommande (CommandeInterpreter* interpreter);

	std::string getShortHelp() const;
	std::string getHelp() const;
};

#endif /* ECHOCOMMAND_H_ */
