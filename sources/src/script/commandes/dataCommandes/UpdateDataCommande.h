/*
 * EchoCommand.h
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#ifndef UPDATEDATACOMMAND_H_
#define UPDATEDATACOMMAND_H_

#include <string>

#include "script/commandes/Commande.h"

#include "script/commandes/dataCommandes/UpdateDataCommande.h"

class UpdateDataCommande : public Commande {
	void executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException);

public:
	UpdateDataCommande(CommandeInterpreter* interpreter);

	std::string getShortHelp() const;
	std::string getHelp() const;
};

#endif /* UPDATEDATACOMMAND_H_ */
