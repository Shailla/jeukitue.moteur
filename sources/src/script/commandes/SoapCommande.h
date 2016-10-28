/*
 * EchoCommand.h
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#ifndef SOAPCOMMAND_H_
#define SOAPCOMMAND_H_

#include <string>

#include "script/commandes/Commande.h"

class SoapCommande : public Commande {
	void executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException);

public:
	SoapCommande (CommandeInterpreter* interpreter);

	std::string getShortHelp() const;
	std::string getHelp() const;
};

#endif /* SOAPCOMMAND_H_ */
