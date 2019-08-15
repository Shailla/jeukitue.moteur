/*
 * Commande.h
 *
 *  Created on: 6 mars 2013
 *      Author: vgdj7997
 */

#ifndef COMMANDE_H_
#define COMMANDE_H_

#include <string>
#include <map>

#include "script/exception/IllegalParameterException.h"

class CommandeInterpreter;

class Commande {
	friend class DataCommande;
	friend class SoapCommande;
	std::map<std::string, Commande*> _subCommandes;

protected:
	CommandeInterpreter* _interpreter;

	void printStdLn(const std::string& msg, bool userOutput);
	void printErrLn(const std::string& msg, bool userOutput);

	virtual void executeIt(std::string ligne, bool userOutput) = 0;

public:
	Commande(CommandeInterpreter* interpreter);
	virtual ~Commande();

	virtual std::string getHelp() const = 0;
	virtual std::string getShortHelp() const = 0;
	void execute(const std::string& ligne, bool userOutput);

	// Gestion des sous-commandes
	Commande* getSubCommande(const std::string& subCommandeName) noexcept(false);
	void registerSubCommande(const std::string& subCommandeName, Commande* subCommande);

	static int getIntParameter(const std::string& word) noexcept(false);
	static float getFloatParameter(const std::string& word) noexcept(false);
	static std::vector<int> getIntParameters(const std::string& ligne) noexcept(false);
};

#endif /* COMMANDE_H_ */
