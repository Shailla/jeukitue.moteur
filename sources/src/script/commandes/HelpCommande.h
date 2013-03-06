/*
 * HelpCommande.h
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#ifndef HELPCOMMANDE_H_
#define HELPCOMMANDE_H_

#include <string>

#include "script/commandes/Commande.h"

class HelpCommande : public Commande {
	void executeIt(std::string ligne, bool userOutput);

public:
	HelpCommande (CommandeInterpreter* interpreter);

	std::string getHelp();
};

#endif /* ECHOCOMMAND_H_ */
