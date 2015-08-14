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
#include "data/Valeur.h"
#include "data/ValeurInt.h"
#include "data/ValeurFloat.h"
#include "data/ValeurString.h"
#include "data/MarqueurDistant.h"

#include "data/Branche.h"

using namespace JktUtils;

Branche::Branche(AbstractBranche* parent, int brancheId, const string& brancheName, DONNEE_TYPE brancheType, int revision, int brancheTmpId) : AbstractBranche(parent, brancheId, brancheName, brancheType, revision, brancheTmpId) {
}

Branche::~Branche() {
}

Branche* Branche::getSubBrancheByName(const string& brancheName) const {
	Branche* branche = NULL;

	try {
		branche = _subBranchesByName.at(brancheName);
	}
	catch(out_of_range& exception) {
		branche = NULL;
	}

	return branche;
}

Branche* Branche::getSubBrancheByIdOrTmpId(int brancheId) const {
	Branche* branche = NULL;

	try {
		if(brancheId >= 0) {
			branche = _subBranchesById.at(brancheId);
		}
		else {
			branche = _subBranchesByTmpId.at(brancheId);
		}
	}
	catch(out_of_range& exception) {
		branche = NULL;
	}

	return branche;
}

Branche* Branche::getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException) {
	if(brancheId >= 0) {
		return getSubBrancheByIdOrTmpId(brancheId);
	}
	else {
		return getSubBrancheByDistantTmpId(distant, brancheId);
	}
}

Branche* Branche::getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheTmpId) throw(NotExistingBrancheException) {
	vector<Branche*>::iterator it;
	MarqueurDistant* marqueur;
	Branche* subBranche = NULL;

	for(it = _subBranches.begin() ; (it!=_subBranches.end() && subBranche==NULL) ; it++) {
		subBranche = *it;
		marqueur = subBranche->getMarqueur(distant);

		if(marqueur->getTemporaryId() != brancheTmpId) {
			subBranche = NULL;
		}
	}

	if(!subBranche) {
		throw NotExistingBrancheException("Branche::getSubBrancheByDistantTmpId 1");
	}

	return subBranche;
}

Valeur* Branche::getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException) {
	vector<Valeur*>::iterator it;
	MarqueurDistant* marqueur;
	Valeur* valeur = NULL;

	for(it = _valeurs.begin() ; (it!=_valeurs.end() && valeur==NULL) ; it++) {
		valeur = *it;
		marqueur = valeur->getMarqueur(distant);

		if(marqueur->getTemporaryId() != valeurTmpId) {
			valeur = NULL;
		}
	}

	if(!valeur) {
		throw NotExistingValeurException("Branche::getValeurByDistantTmpId 2");
	}

	return valeur;
}

Valeur* Branche::getValeurByIdOrTmpId(int valeurId) const {
	Valeur* valeur = NULL;

	try {
		if(valeurId >= 0) {
			valeur = _valeursById.at(valeurId);
		}
		else {
			valeur = _valeursByTmpId.at(valeurId);
		}

	}
	catch(out_of_range& exception) {
		valeur = NULL;
	}

	return valeur;
}

Branche* Branche::createSubBrancheForClient(const string& brancheName, int revision) {
	// Alloue une référence pour la nouvelle branche
	int tmpRef = -_brancheTmpRefGenerator.genRef();		// On démarre à -1

	// Crée la nouvelle branche
	Branche* newBranche = new Branche(this, -1, brancheName, getDonneeType(), revision, tmpRef);
	_subBranchesByTmpId[tmpRef] = newBranche;		// Moins 1 pour éviter les collisions avec les ID non temporaires
	_subBranches.push_back(newBranche);

	return newBranche;
}


Branche* Branche::createSubBrancheForServer(const string& brancheName, DONNEE_TYPE type, int revision) throw(AlreadyExistingBrancheException) {
	// Alloue une référence pour la nouvelle branche
	int ref = _brancheRefGenerator.genRef();	// On démarre à 1

	// Vérifie si la branche n'existe pas déjà
	bool brancheExisting = true;

	try {
		_subBranchesByName.at(brancheName);
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

	_subBranchesById[ref] = newBranche;
	_subBranchesByName[brancheName] = newBranche;
	_subBranches.push_back(newBranche);

	return newBranche;
}

Branche* Branche::addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision) {
	Branche* newBranche;
	map<int, Branche*>::iterator iter = _subBranchesById.find(brancheId);

	if(iter != _subBranchesById.end()) {
		cerr << endl << __FILE__ << ":" << __LINE__ << "La branche " << brancheId << " (" << brancheName << ") de parent " << CollectionsUtils::toString(getBrancheFullId()) << " existe déjà";
		newBranche = iter->second;
	}
	else {
		// Crée la nouvelle branche
		newBranche = new Branche(this, brancheId, brancheName, getDonneeType(), brancheRevision, -1);
		_subBranchesById[brancheId] = newBranche;
		_subBranchesByName[brancheName] = newBranche;
		_subBranches.push_back(newBranche);
	}

	return newBranche;
}

Branche* Branche::acceptTmpSubBranche(int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException) {
	Branche* branche = getSubBrancheByIdOrTmpId(brancheTmpId);

	if(!branche) {
		throw NotExistingBrancheException("Branche::acceptTmpSubBranche");
	}

	_subBranchesById[brancheId] = branche;
	_subBranchesByName[branche->getBrancheName()] = branche;
	branche->setBrancheId(brancheId);
	branche->setRevision(brancheRevision);

	return branche;
}

Valeur* Branche::acceptTmpValeur(int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException) {
	Valeur* valeur = getValeurByIdOrTmpId(valeurTmpId);

	if(!valeur) {
		throw NotExistingBrancheException("Branche::acceptTmpValeur");
	}

	_valeursById[valeurId] = valeur;
	valeur->setValeurId(valeurId);
	valeur->setRevision(valeurRevision);

	return valeur;
}

Valeur* Branche::createValeurForClient(const string& valeurName, int revision, const AnyData& valeur) {
	// Alloue une référence pour la nouvelle branche
	int tmpRef = -_valeurTmpRefGenerator.genRef();		// On démarre à -1

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
		_valeursByTmpId[tmpRef] = newValeur;
		_valeurs.push_back(newValeur);
	}

	return newValeur;
}

Valeur* Branche::createValeurForServeur(const string& valeurName, int revision, const AnyData& valeur) {
	// Alloue une référence pour la nouvelle branche
	int ref = _valeurRefGenerator.genRef();		// On démarre à 1

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
		_valeursById[ref] = newValeur;
		_valeurs.push_back(newValeur);
	}

	return newValeur;
}

const Valeur* Branche::addValeur(int valeurId, const string& valeurName, int valeurRevision, const JktUtils::AnyData& valeur) {
	Valeur* newValeur = getValeurByIdOrTmpId(valeurId);

	if(newValeur) {
		cout << endl << __FILE__ << ":" << __LINE__ << "La valeur " << valeurId << " (" << valeurName << ") de parent " << CollectionsUtils::toString(getBrancheFullId()) << " existe déjà";
	}
	else {
		// Crée la nouvelle valeur
		if(valeur.isInt()) {
			newValeur = new ValeurInt(this, valeurId, valeurName, DONNEE_DEFAULT, -1, valeurRevision, valeur.getValueInt());
			_valeursById[valeurId] = newValeur;
			_valeurs.push_back(newValeur);
		}
		else if(valeur.isFloat()) {
			newValeur = new ValeurFloat(this, valeurId, valeurName, DONNEE_DEFAULT, -1, valeurRevision, valeur.getValueFloat());
			_valeursById[valeurId] = newValeur;
			_valeurs.push_back(newValeur);
		}
		else if(valeur.isString()) {
			newValeur = new ValeurString(this, valeurId, valeurName, DONNEE_DEFAULT, -1, valeurRevision, valeur.getValueString());
			_valeursById[valeurId] = newValeur;
			_valeurs.push_back(newValeur);
		}
		else {
			cerr << endl << __FILE__ << ":" << __LINE__ << " Type de valeur inconnu";
		}
	}

	return newValeur;
}

vector<Branche*>& Branche::getSubBranches() {
	return _subBranches;
}

vector<Valeur*>& Branche::getValeurs() {
	return _valeurs;
}

void Branche::print(ostringstream& out, bool details, int indentation) {
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

	// Affiche les valeurs de la branche de manière ordonnée
	vector<Valeur*> valeurs = _valeurs;
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
	vector<Branche*> branches = _subBranches;
	sort(branches.begin(), branches.end(), AbstractBranche::highestId);

	vector<Branche*>::iterator brIt;

	for(brIt = branches.begin() ; brIt != branches.end() ; ++brIt) {
		Branche* branche = *brIt;
		branche->print(out, details, indentation+1);
	}
}
