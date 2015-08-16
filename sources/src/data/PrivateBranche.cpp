/*
 * Branche.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

#include "util/CollectionsUtils.h"
#include "data/Branche.h"
#include "data/Valeur.h"
#include "data/ValeurInt.h"
#include "data/ValeurFloat.h"
#include "data/ValeurString.h"
#include "data/MarqueurDistant.h"
#include "data/exception/UnavailableOperationException.h"

#include "data/PrivateBranche.h"

using namespace JktUtils;

PrivateBranche::PrivateBranche(AbstractBranche* parent, int brancheId, const string& brancheName, DONNEE_TYPE brancheType, int revision, int brancheTmpId) : Branche(parent, brancheId, brancheName, brancheType, revision, brancheTmpId) {
}

PrivateBranche::~PrivateBranche() {
}

map<DistantTreeProxy*, PrivateBranche::DistantPrivateBranche>& PrivateBranche::getDistants() {
	return _distants;
}

PrivateBranche::DistantPrivateBranche& PrivateBranche::getDistant(DistantTreeProxy* distant) {
	return _distants[distant];
}

Branche* PrivateBranche::getSubBrancheByName(DistantTreeProxy* distant, const string& brancheName) {
	Branche* branche = NULL;

	try {
		branche = getDistant(distant)._subBranchesByName.at(brancheName);
	}
	catch(out_of_range& exception) {
		branche = NULL;
	}

	return branche;
}

Branche* PrivateBranche::getSubBrancheByIdOrTmpId(DistantTreeProxy* distant, int brancheId) {
	Branche* branche = NULL;

	PrivateBranche::DistantPrivateBranche& prBr = getDistant(distant);

	try {
		if(brancheId >= 0) {
			branche = prBr._subBranchesById.at(brancheId);
		}
		else {
			branche = prBr._subBranchesByTmpId.at(brancheId);
		}
	}
	catch(out_of_range& exception) {
		branche = NULL;
	}

	return branche;
}

Branche* PrivateBranche::getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException) {
	if(brancheId >= 0) {
		return getSubBrancheByIdOrTmpId(distant, brancheId);
	}
	else {
		return getSubBrancheByDistantTmpId(distant, brancheId);
	}
}

Branche* PrivateBranche::getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheTmpId) throw(NotExistingBrancheException) {
	vector<Branche*>::iterator it;
	MarqueurDistant* marqueur;
	Branche* subBranche = NULL;

	for(it = getDistant(distant)._subBranches.begin() ; (it!=getDistant(distant)._subBranches.end() && subBranche==NULL) ; it++) {
		subBranche = *it;
		marqueur = subBranche->getMarqueur(distant);

		if(marqueur->getTemporaryId() != brancheTmpId) {
			subBranche = NULL;
		}
	}

	if(!subBranche) {
		throw NotExistingBrancheException("PrivateBranche::getSubBrancheByDistantTmpId 1");
	}

	return subBranche;
}

Valeur* PrivateBranche::getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException) {
	vector<Valeur*>::iterator it;
	MarqueurDistant* marqueur;
	Valeur* valeur = NULL;

	for(it = getDistant(distant)._valeurs.begin() ; (it!=getDistant(distant)._valeurs.end() && valeur==NULL) ; it++) {
		valeur = *it;
		marqueur = valeur->getMarqueur(distant);

		if(marqueur->getTemporaryId() != valeurTmpId) {
			valeur = NULL;
		}
	}

	if(!valeur) {
		throw NotExistingValeurException("PrivateBranche::getValeurByDistantTmpId 2");
	}

	return valeur;
}

Valeur* PrivateBranche::getValeurByIdOrTmpId(DistantTreeProxy* distant, int valeurId) {
	Valeur* valeur = NULL;

	try {
		if(valeurId >= 0) {
			valeur = getDistant(distant)._valeursById.at(valeurId);
		}
		else {
			valeur = getDistant(distant)._valeursByTmpId.at(valeurId);
		}

	}
	catch(out_of_range& exception) {
		valeur = NULL;
	}

	return valeur;
}

Branche* PrivateBranche::createSubBrancheForClient(DistantTreeProxy* distant, const string& brancheName, int revision) {
	PrivateBranche::DistantPrivateBranche& dst = getDistant(distant);

	// Alloue une référence pour la nouvelle branche
	int tmpRef = -dst._brancheTmpRefGenerator.genRef();		// On démarre à -1

	// Crée la nouvelle branche
	Branche* newBranche = new Branche(this, -1, brancheName, getDonneeType(), revision, tmpRef);
	dst._subBranchesByTmpId[tmpRef] = newBranche;		// Moins 1 pour éviter les collisions avec les ID non temporaires
	dst._subBranches.push_back(newBranche);

	return newBranche;
}

Branche* PrivateBranche::createSubBrancheForServer(DistantTreeProxy* distant, const string& brancheName, DONNEE_TYPE type, int revision) throw(AlreadyExistingBrancheException) {
	PrivateBranche::DistantPrivateBranche& dst = getDistant(distant);

	// Alloue une référence pour la nouvelle branche
	int ref = dst._brancheRefGenerator.genRef();	// On démarre à 1

	// Vérifie si la branche n'existe pas déjà
	bool brancheExisting = true;

	try {
		dst._subBranchesByName.at(brancheName);
	}
	catch(out_of_range& exception) {
		brancheExisting = false;
	}

	if(brancheExisting) {
		stringstream message;
		message << "La branche nommée '" << brancheName << "' de parent " << CollectionsUtils::toString(getBrancheFullId()) << " existe déjà";
		throw AlreadyExistingBrancheException(message.str());
	}

	// Crée la nouvelle branche
	DONNEE_TYPE resultType;
	DONNEE_TYPE parentType = getDonneeType();

	if(parentType == DONNEE_SERVER) {		// Sur une branche serveur on peut créer une branche locale ou serveur
		if(type == DONNEE_CLIENT) {
			resultType = DONNEE_CLIENT;
		}
		else {
			resultType = DONNEE_SERVER;
		}
	}
	else if(parentType == DONNEE_CLIENT || parentType == DONNEE_CLIENT_SUB) {	// Sur une branche client on ne peut créer qu'une donnée client fille
		resultType = DONNEE_CLIENT_SUB;
	}
	else if(parentType == DONNEE_LOCAL) {	// Sur une branche local on ne peut créer qu'une donnée local
		resultType = DONNEE_LOCAL;
	}
	else {
		cerr << endl << "Type demandé inconnu";
	}

	Branche* newBranche = new Branche(this, ref, brancheName, resultType, revision, -1);

	getDistant(distant)._subBranchesById[ref] = newBranche;
	getDistant(distant)._subBranchesByName[brancheName] = newBranche;
	getDistant(distant)._subBranches.push_back(newBranche);

	return newBranche;
}

Branche* PrivateBranche::addSubBranche(DistantTreeProxy* distant, int brancheId, const std::string& brancheName, int brancheRevision) {
	Branche* newBranche;
	map<int, Branche*>::iterator iter = getDistant(distant)._subBranchesById.find(brancheId);

	if(iter != getDistant(distant)._subBranchesById.end()) {
		cerr << endl << __FILE__ << ":" << __LINE__ << "La branche " << brancheId << " (" << brancheName << ") de parent " << CollectionsUtils::toString(getBrancheFullId()) << " existe déjà";
		newBranche = iter->second;
	}
	else {
		// Crée la nouvelle branche
		newBranche = new Branche(this, brancheId, brancheName, getDonneeType(), brancheRevision, -1);
		getDistant(distant)._subBranchesById[brancheId] = newBranche;
		getDistant(distant)._subBranchesByName[brancheName] = newBranche;
		getDistant(distant)._subBranches.push_back(newBranche);
	}

	return newBranche;
}

Branche* PrivateBranche::acceptTmpSubBranche(DistantTreeProxy* distant, int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException) {
	Branche* branche = getSubBrancheByIdOrTmpId(distant, brancheTmpId);

	if(!branche) {
		throw NotExistingBrancheException("PrivateBranche::acceptTmpSubBranche");
	}

	getDistant(distant)._subBranchesById[brancheId] = branche;
	getDistant(distant)._subBranchesByName[branche->getBrancheName()] = branche;
	branche->setBrancheId(brancheId);
	branche->setRevision(brancheRevision);

	return branche;
}

Valeur* PrivateBranche::acceptTmpValeur(DistantTreeProxy* distant, int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException) {
	Valeur* valeur = getValeurByIdOrTmpId(distant, valeurTmpId);

	if(!valeur) {
		throw NotExistingBrancheException("PrivateBranche::acceptTmpValeur");
	}

	getDistant(distant)._valeursById[valeurId] = valeur;
	valeur->setValeurId(valeurId);
	valeur->setRevision(valeurRevision);

	return valeur;
}

Valeur* PrivateBranche::createValeurForClient(DistantTreeProxy* distant, const string& valeurName, int revision, const AnyData& valeur) {
	PrivateBranche::DistantPrivateBranche& dst = getDistant(distant);

	// Alloue une référence pour la nouvelle branche
	int tmpRef = -dst._valeurTmpRefGenerator.genRef();		// On démarre à -1

	// Crée la nouvelle branche
	Valeur* newValeur = NULL;

	if(valeur.isInt()) {
		newValeur = new ValeurInt(this, -1, valeurName, DONNEE_DEFAULT, tmpRef, revision, valeur.getValueInt());
	}
	else if(valeur.isFloat()) {
		newValeur = new ValeurFloat(this, -1, valeurName, DONNEE_DEFAULT, tmpRef, revision, valeur.getValueFloat());
	}
	else if(valeur.isString()) {
		newValeur = new ValeurString(this, -1, valeurName, DONNEE_DEFAULT, tmpRef, revision, valeur.getValueString());
	}

	if(newValeur) {
		dst._valeursByTmpId[tmpRef] = newValeur;
		dst._valeurs.push_back(newValeur);
	}

	return newValeur;
}

Valeur* PrivateBranche::createValeurForServeur(DistantTreeProxy* distant, const string& valeurName, int revision, const AnyData& valeur) {
	PrivateBranche::DistantPrivateBranche& dst = getDistant(distant);

	// Alloue une référence pour la nouvelle branche
	int ref = dst._valeurRefGenerator.genRef();		// On démarre à 1

	// Crée la nouvelle branche
	Valeur* newValeur = NULL;

	if(valeur.isInt()) {
		newValeur = new ValeurInt(this, ref, valeurName, DONNEE_DEFAULT, -1, revision, valeur.getValueInt());
	}
	else if(valeur.isFloat()) {
		newValeur = new ValeurFloat(this, ref, valeurName, DONNEE_DEFAULT, -1, revision, valeur.getValueFloat());
	}
	else if(valeur.isString()) {
		newValeur = new ValeurString(this, ref, valeurName, DONNEE_DEFAULT, -1, revision, valeur.getValueString());
	}

	if(newValeur) {
		dst._valeursById[ref] = newValeur;
		dst._valeurs.push_back(newValeur);
	}

	return newValeur;
}

const Valeur* PrivateBranche::addValeur(DistantTreeProxy* distant, int valeurId, const string& valeurName, int valeurRevision, const JktUtils::AnyData& valeur) {
	PrivateBranche::DistantPrivateBranche& dst = getDistant(distant);
	Valeur* newValeur = getValeurByIdOrTmpId(distant, valeurId);

	if(newValeur) {
		cout << endl << __FILE__ << ":" << __LINE__ << "La valeur " << valeurId << " (" << valeurName << ") de parent " << CollectionsUtils::toString(getBrancheFullId()) << " existe déjà";
	}
	else {
		// Crée la nouvelle valeur
		if(valeur.isInt()) {
			newValeur = new ValeurInt(this, valeurId, valeurName, DONNEE_DEFAULT, -1, valeurRevision, valeur.getValueInt());
			dst._valeursById[valeurId] = newValeur;
			dst._valeurs.push_back(newValeur);
		}
		else if(valeur.isFloat()) {
			newValeur = new ValeurFloat(this, valeurId, valeurName, DONNEE_DEFAULT, -1, valeurRevision, valeur.getValueFloat());
			dst._valeursById[valeurId] = newValeur;
			dst._valeurs.push_back(newValeur);
		}
		else if(valeur.isString()) {
			newValeur = new ValeurString(this, valeurId, valeurName, DONNEE_DEFAULT, -1, valeurRevision, valeur.getValueString());
			dst._valeursById[valeurId] = newValeur;
			dst._valeurs.push_back(newValeur);
		}
		else {
			cerr << endl << __FILE__ << ":" << __LINE__ << " Type de valeur inconnu";
		}
	}

	return newValeur;
}

vector<Branche*>& PrivateBranche::getSubBranches(DistantTreeProxy* distant) {
	return (vector<Branche*>&)getDistant(distant)._subBranches;
}

vector<Valeur*>& PrivateBranche::getValeurs(DistantTreeProxy* distant) {
	return getDistant(distant)._valeurs;
}

void PrivateBranche::print(ostringstream& out, bool details, int indentation) {
	const char* TABULATION = "\t";
	int i;

	// Affiche la branche
	out << endl;

	for(i = 0 ; i < indentation ; i++) {
		out << TABULATION;
	}

	out << "+ [id=" << _brancheId;
	if(details) {
		out << " tmpId=" << _brancheTmpId;
	}
	out << " rv=" << getRevision() << "] '" << _brancheName << "'";

	std::map<DistantTreeProxy*, DistantPrivateBranche>::iterator itDt;

	for(itDt = _distants.begin() ; itDt != _distants.end() ; itDt++) {
		// Affiche les valeurs de la branche de manière ordonnée
		vector<Valeur*>& valeurs = getDistant(itDt->first)._valeurs;

		sort(valeurs.begin(), valeurs.end(), Valeur::highestId);

		vector<Valeur*>::iterator valIt;

		for(valIt = valeurs.begin() ; valIt != valeurs.end() ; ++valIt) {
			Valeur* valeur = *valIt;

			out << endl;

			for(i = 0 ; i < indentation+1 ; i++) {
				out << TABULATION;
			}

			out << ". [id=" << valeur->getValeurId();
			if(details) {
				out << " tmpId=" << valeur->getValeurTmpId();
			}

			out << " rv=" << valeur->getRevision() << "] '" << valeur->getValeurName() << "' <" << valeur->toString() << ">";
		}

		// Affiche les branches de la branche
		vector<Branche*>& branches = getDistant(itDt->first)._subBranches;
		sort(branches.begin(), branches.end(), AbstractBranche::highestId);

		vector<Branche*>::iterator brIt;

		for(brIt = branches.begin() ; brIt != branches.end() ; ++brIt) {
			Branche* branche = *brIt;
			branche->print(out, details, indentation+1);
		}
	}
}

Branche* PrivateBranche::createSubBrancheForServer(const std::string& brancheName, DONNEE_TYPE type, int revision) throw(AlreadyExistingBrancheException) {
	throw UnavailableOperationException("Private branche need a distant for operation createSubBrancheForServer");
}

Valeur* PrivateBranche::createValeurForServeur(const std::string& valeurName, int revision, const JktUtils::AnyData& valeur) {
	throw UnavailableOperationException("Private branche need a distant for operation createValeurForServeur");
}

Branche* PrivateBranche::getSubBrancheByName(const string& brancheName) const {
	throw UnavailableOperationException("Private branche need a distant for operation getSubBrancheByName");
}

Branche* PrivateBranche::getSubBrancheByIdOrTmpId(int brancheId) const {
	throw UnavailableOperationException("Private branche need a distant for operation getSubBrancheByIdOrTmpId");
}

std::vector<Branche*>& PrivateBranche::getSubBranches() {
	throw UnavailableOperationException("Private branche need a distant for operation getSubBranches");
}

std::vector<Valeur*>& PrivateBranche::getValeurs() {
	throw UnavailableOperationException("Private branche need a distant for operation getValeurs");
}

Valeur* PrivateBranche::getValeurByIdOrTmpId(int valeurId) const {
	throw UnavailableOperationException("Private branche need a distant for operation getValeurByIdOrTmpId");
}

Branche* PrivateBranche::acceptTmpSubBranche(int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException) {
	throw UnavailableOperationException("Private branche need a distant for operation acceptTmpSubBranche");
}

Valeur* PrivateBranche::acceptTmpValeur(int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException) {
	throw UnavailableOperationException("Private branche need a distant for operation acceptTmpValeur");
}

Branche* PrivateBranche::addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision) {
	throw UnavailableOperationException("Private branche need a distant for operation addSubBranche");
}

const Valeur* PrivateBranche::addValeur(int valeurId, const std::string& valeurName, int valeurRevision, const JktUtils::AnyData& valeur) {
	throw UnavailableOperationException("Private branche need a distant for operation addValeur");
}
