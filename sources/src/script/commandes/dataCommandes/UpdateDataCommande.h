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

#include "script/commandes/dataCommandes/UpdateDataCommande.h"

class UpdateDataCommande : public Commande {
	void executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException);

public:
	UpdateDataCommande(CommandeInterpreter* interpreter);

	std::string getHelp();
};

#endif /* ECHOCOMMAND_H_ */
