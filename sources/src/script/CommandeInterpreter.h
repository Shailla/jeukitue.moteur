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

#include "menu/Viewer.h"

class Viewer;
class Commande;

class CommandeInterpreter {
	friend class Commande;

	std::map<std::string, Commande*> _commandes;
	Viewer* _viewer;

protected:
	void printStdLn(const std::string& msg, bool userOutput);
	void printErrLn(const std::string& msg, bool userOutput);

public:
	CommandeInterpreter(Viewer* viewer);
	virtual ~CommandeInterpreter();

	Commande* getCommande(const std::string& commandeName);

	void interpreteCommande(std::string commande, bool userOutput);
};

#endif /* COMMANDEINTERPRETER_H_ */
