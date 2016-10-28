/*
 * StopSoapCommande.h
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#ifndef SOAPSTOPCOMMAND_H_
#define SOAPSTOPCOMMAND_H_

#include <string>

#include "script/commandes/Commande.h"

class StopSoapCommande : public Commande {
	void executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException);

public:
	StopSoapCommande (CommandeInterpreter* interpreter);

	std::string getShortHelp() const;
	std::string getHelp() const;
};

#endif /* SOAPSTOPCOMMAND_H_ */
