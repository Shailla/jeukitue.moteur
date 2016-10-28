/*
 * EchoCommand.h
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#ifndef SOAPSTARTCOMMAND_H_
#define SOAPSTARTCOMMAND_H_

#include <string>

#include "script/commandes/Commande.h"

class StartSoapCommande : public Commande {
	void executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException);

public:
	StartSoapCommande (CommandeInterpreter* interpreter);

	std::string getShortHelp() const;
	std::string getHelp() const;
};

#endif /* SOAPSTARTCOMMAND_H_ */
