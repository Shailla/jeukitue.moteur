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
#include "data/PrivateBranche.h"

using namespace JktUtils;


BrancheIterator::BrancheIterator(Branche* origin, DistantTreeProxy* distant) {
	_origin = origin;
	_distant = distant;
}

void BrancheIterator::push(Branche* br, int pos, int size) {
	_br.push(br);
	_pos.push(pos);
	_size.push(size);
}

void BrancheIterator::pop() {
	_br.pop();
	_pos.pop();
	_size.pop();
}

bool BrancheIterator::operator++() {
	Branche* br;
	int pos, size;

	if(_br.empty()) {
		vector<Branche*>* subBranches = _origin->getSubBranches(_distant);
		push(_origin, 0, subBranches->size());

		return true;
	}

	// Essaie d'avancer dans l'arbre
	pos = _pos.top();
	size = _size.top();

	if(pos < size) {
		// Avance
		br = _br.top();
		vector<Branche*>* subBranches = br->getSubBranches(_distant);

		Branche* subBranche = subBranches->operator [](pos);

		if(subBranche->getSubBranches(_distant)) {
			push(subBranche, 0, subBranche->getSubBranches(_distant)->size());
		}
		else {
			push(subBranche, 0, 0);		// Pas de sous-branches
		}

		return true;
	}

	// Essaie de descendre, sinon recule jusqu'à pouvoir descendre ou avoir fini
	while(_br.size()) {
		pos = _pos.top();
		size = _size.top();
		br = _br.top();

		if(pos+1 < size) {
			// Descend
			pop();
			push(br, pos+1, size);

			// Et avance
			vector<Branche*>* subBranches = br->getSubBranches(_distant);

			Branche* subBranche = subBranches->operator [](pos+1);

			if(subBranche->getSubBranches(_distant)) {
				push(subBranche, 0, subBranche->getSubBranches(_distant)->size());
			}
			else {
				push(subBranche, 0, 0);		// Pas de sous-branches
			}

			return true;
		}
		else {
			// Recule
			pop();
		}
	}

	return false;
}

Branche* BrancheIterator::operator*() const {
	return _br.top();
}



Branche::Branche(DataTree* dataTree, int brancheId, DONNEE_TYPE brancheType, int revision, int brancheTmpId) : AbstractBranche(dataTree, brancheId, "root", brancheType, revision, brancheTmpId) {
}

Branche::Branche(AbstractBranche* parent, int brancheId, const string& brancheName, DONNEE_TYPE brancheType, int revision, int brancheTmpId) : AbstractBranche(parent, brancheId, brancheName, brancheType, revision, brancheTmpId) {
}

Branche::~Branche() {
}

Branche* Branche::getSubBrancheByName(DistantTreeProxy* distant, const string& brancheName) {
	Branche* branche = 0;

	try {
		branche = _subBranchesByName.at(brancheName);
	}
	catch(out_of_range& exception) {
		branche = NULL;
	}

	return branche;
}

Valeur* Branche::getValeurByName(DistantTreeProxy* distant, const string& valeurName) {
	Valeur* valeur = NULL;

	try {
		valeur = _valeursByName.at(valeurName);
	}
	catch(out_of_range& exception) {
		valeur = 0;
	}

	return valeur;
}

Branche* Branche::getSubBrancheByIdOrTmpId(DistantTreeProxy* distant, int brancheId) {
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

BrancheIterator Branche::begin(DistantTreeProxy* distant) {
	return BrancheIterator(this, distant);
}

Branche* Branche::getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException) {
	if(brancheId >= 0) {
		return getSubBrancheByIdOrTmpId(distant, brancheId);
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

Valeur* Branche::getValeurByIdOrTmpId(DistantTreeProxy* distant, int valeurId) {
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
	Branche* newBranche = new Branche(this, -1, brancheName, getDonneeType(), revision, tmpRef);
	_subBranchesByTmpId[tmpRef] = newBranche;		// Moins 1 pour éviter les collisions avec les ID non temporaires
	_subBranches.push_back(newBranche);
	_subBranchesByName[brancheName] = newBranche;

	return newBranche;
}


Branche* Branche::createSubBrancheForServer(DistantTreeProxy* distant, const string& brancheName, DONNEE_TYPE type, int revision) throw(AlreadyExistingBrancheException) {
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

	if(parentType == DONNEE_PUBLIC) {		// Sur une branche serveur on peut créer une branche locale ou serveur
		if(type == DONNEE_PRIVATE) {
			resultType = DONNEE_PRIVATE;
		}
		else {
			resultType = DONNEE_PUBLIC;
		}
	}
	else if(parentType == DONNEE_PRIVATE || parentType == DONNEE_PRIVATE_SUB) {	// Sur une branche client on ne peut créer qu'une donnée client fille
		resultType = DONNEE_PRIVATE_SUB;
	}
	else if(parentType == DONNEE_LOCAL) {	// Sur une branche local on ne peut créer qu'une donnée local
		resultType = DONNEE_LOCAL;
	}
	else {
		cerr << endl << "Type parent inconnu";
		resultType = DONNEE_DEFAULT;
	}

	Branche* newBranche;

	if(resultType == DONNEE_PRIVATE) {
		newBranche = new PrivateBranche(this, ref, brancheName, resultType, revision, -1);
	}
	else {
		newBranche = new Branche(this, ref, brancheName, resultType, revision, -1);
	}

	_subBranchesById[ref] = newBranche;
	_subBranchesByName[brancheName] = newBranche;
	_subBranches.push_back(newBranche);

	return newBranche;
}

Branche* Branche::addSubBranche(DistantTreeProxy* distant, int brancheId, DONNEE_TYPE type, const std::string& brancheName, int brancheRevision) {
	Branche* newBranche;
	map<int, Branche*>::iterator iter = _subBranchesById.find(brancheId);

	if(iter != _subBranchesById.end()) {
		cerr << endl << __FILE__ << ":" << __LINE__ << "La branche " << brancheId << " (" << brancheName << ") de parent " << CollectionsUtils::toString(getBrancheFullId()) << " existe déjà";
		newBranche = iter->second;
	}
	else {
		// Crée la nouvelle branche
		DONNEE_TYPE resultType;
		DONNEE_TYPE parentType = getDonneeType();

		if(parentType == DONNEE_PUBLIC) {		// Sur une branche serveur on peut créer une branche locale ou serveur
			if(type == DONNEE_PRIVATE) {
				resultType = DONNEE_PRIVATE;
			}
			else {
				resultType = DONNEE_PUBLIC;
			}
		}
		else if(parentType == DONNEE_PRIVATE || parentType == DONNEE_PRIVATE_SUB) {	// Sur une branche client on ne peut créer qu'une donnée client fille
			resultType = DONNEE_PRIVATE_SUB;
		}
		else if(parentType == DONNEE_LOCAL) {	// Sur une branche local on ne peut créer qu'une donnée local
			resultType = DONNEE_LOCAL;
		}
		else {
			cerr << endl << "Type parent inconnu";
			resultType = DONNEE_DEFAULT;
		}

		// Crée la nouvelle branche
		newBranche = new Branche(this, brancheId, brancheName, resultType, brancheRevision, -1);
		_subBranchesById[brancheId] = newBranche;
		_subBranchesByName[brancheName] = newBranche;
		_subBranches.push_back(newBranche);
	}

	return newBranche;
}

Branche* Branche::acceptTmpSubBranche(DistantTreeProxy* distant, int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException) {
	Branche* branche = getSubBrancheByIdOrTmpId(0, brancheTmpId);

	if(!branche) {
		throw NotExistingBrancheException("Branche::acceptTmpSubBranche");
	}

	_subBranchesById[brancheId] = branche;
	_subBranchesByName[branche->getBrancheName()] = branche;
	branche->setBrancheId(brancheId);
	branche->setRevision(brancheRevision);

	return branche;
}

Valeur* Branche::acceptTmpValeur(DistantTreeProxy* distant, int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException) {
	Valeur* valeur = getValeurByIdOrTmpId(distant, valeurTmpId);

	if(!valeur) {
		throw NotExistingBrancheException("Branche::acceptTmpValeur");
	}

	_valeursById[valeurId] = valeur;
	valeur->setValeurId(valeurId);
	valeur->setRevision(valeurRevision);

	return valeur;
}

Valeur* Branche::createValeurForClient(UPDATE_MODE updateMode, const string& valeurName, int revision, const AnyData& valeur) {
	// Alloue une référence pour la nouvelle branche
	int tmpRef = -_valeurTmpRefGenerator.genRef();		// On démarre à -1

	// Détermine le type de la nouvelle valeur à créer
	DONNEE_TYPE resultType;
	DONNEE_TYPE parentType = getDonneeType();

	if(parentType == DONNEE_PUBLIC) {		// Sur une branche serveur on peut créer une branche locale ou serveur
		resultType = DONNEE_PUBLIC;
	}
	else if(parentType == DONNEE_PRIVATE || parentType == DONNEE_PRIVATE_SUB) {	// Sur une branche client on ne peut créer qu'une donnée client fille
		resultType = DONNEE_PRIVATE_SUB;
	}
	else if(parentType == DONNEE_LOCAL) {	// Sur une branche local on ne peut créer qu'une donnée local
		resultType = DONNEE_LOCAL;
	}
	else {
		cerr << endl << "Type parent inconnu";
		resultType = DONNEE_DEFAULT;
	}

	// Crée la nouvelle branche
	Valeur* newValeur = NULL;

	if(valeur.isInt()) {
		newValeur = new ValeurInt(this, -1, valeurName, resultType, updateMode, tmpRef, revision, valeur.getValueInt());
	}
	else if(valeur.isFloat()) {
		newValeur = new ValeurFloat(this, -1, valeurName, resultType, updateMode, tmpRef, revision, valeur.getValueFloat());
	}
	else if(valeur.isString()) {
		newValeur = new ValeurString(this, -1, valeurName, resultType, updateMode, tmpRef, revision, valeur.getValueString());
	}

	if(newValeur) {
		_valeursByTmpId[tmpRef] = newValeur;
		_valeurs.push_back(newValeur);
	}

	return newValeur;
}

Valeur* Branche::createValeurForServeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const string& valeurName, int revision, const AnyData& valeur) {
	// Alloue une référence pour la nouvelle branche
	int ref = _valeurRefGenerator.genRef();		// On démarre à 1

	// Détermine le type de la nouvelle valeur à créer
	DONNEE_TYPE resultType;
	DONNEE_TYPE parentType = getDonneeType();

	if(parentType == DONNEE_PUBLIC) {		// Sur une branche serveur on peut créer une branche locale ou serveur
		resultType = DONNEE_PUBLIC;
	}
	else if(parentType == DONNEE_PRIVATE || parentType == DONNEE_PRIVATE_SUB) {	// Sur une branche client on ne peut créer qu'une donnée client fille
		resultType = DONNEE_PRIVATE_SUB;
	}
	else if(parentType == DONNEE_LOCAL) {	// Sur une branche local on ne peut créer qu'une donnée local
		resultType = DONNEE_LOCAL;
	}
	else {
		cerr << endl << "Type parent inconnu";
		resultType = DONNEE_DEFAULT;
	}

	// Crée la nouvelle branche
	Valeur* newValeur = NULL;

	if(valeur.isInt()) {
		newValeur = new ValeurInt(this, ref, valeurName, resultType, updateMode, -1, revision, valeur.getValueInt());
	}
	else if(valeur.isFloat()) {
		newValeur = new ValeurFloat(this, ref, valeurName, resultType, updateMode, -1, revision, valeur.getValueFloat());
	}
	else if(valeur.isString()) {
		newValeur = new ValeurString(this, ref, valeurName, resultType, updateMode, -1, revision, valeur.getValueString());
	}

	if(newValeur) {
		_valeursById[ref] = newValeur;
		_valeurs.push_back(newValeur);
	}

	return newValeur;
}

Valeur* Branche::addValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, int valeurId, const string& valeurName, int valeurRevision, const JktUtils::AnyData& valeur) {
	Valeur* newValeur = getValeurByIdOrTmpId(distant, valeurId);

	if(newValeur) {
		cout << endl << __FILE__ << ":" << __LINE__ << "La valeur " << valeurId << " (" << valeurName << ") de parent " << CollectionsUtils::toString(getBrancheFullId()) << " existe déjà";
	}
	else {
		// Détermine le type de la nouvelle valeur à créer
		DONNEE_TYPE resultType;
		DONNEE_TYPE parentType = getDonneeType();

		if(parentType == DONNEE_PUBLIC) {		// Sur une branche serveur on peut créer une branche locale ou serveur
			resultType = DONNEE_PUBLIC;
		}
		else if(parentType == DONNEE_PRIVATE || parentType == DONNEE_PRIVATE_SUB) {	// Sur une branche client on ne peut créer qu'une donnée client fille
			resultType = DONNEE_PRIVATE_SUB;
		}
		else if(parentType == DONNEE_LOCAL) {	// Sur une branche local on ne peut créer qu'une donnée local
			resultType = DONNEE_LOCAL;
		}
		else {
			cerr << endl << "Type parent inconnu";
			resultType = DONNEE_DEFAULT;
		}

		// Crée la nouvelle valeur
		if(valeur.isInt()) {
			newValeur = new ValeurInt(this, valeurId, valeurName, resultType, updateMode, -1, valeurRevision, valeur.getValueInt());
		}
		else if(valeur.isFloat()) {
			newValeur = new ValeurFloat(this, valeurId, valeurName, resultType, updateMode, -1, valeurRevision, valeur.getValueFloat());
		}
		else if(valeur.isString()) {
			newValeur = new ValeurString(this, valeurId, valeurName, resultType, updateMode, -1, valeurRevision, valeur.getValueString());
		}
		else {
			newValeur = 0;
			cerr << endl << __FILE__ << ":" << __LINE__ << " Type de valeur inconnu";
		}

		if(newValeur) {
			_valeursById[valeurId] = newValeur;
			_valeursByName[valeurName] = newValeur;
			_valeurs.push_back(newValeur);
		}
	}

	return newValeur;
}

vector<Branche*>* Branche::getSubBranches(DistantTreeProxy* distant) {
	return &_subBranches;
}

vector<Valeur*>& Branche::getValeurs(DistantTreeProxy* distant) {
	return _valeurs;
}

void Branche::print(ostringstream& out, DistantTreeProxy* distant, bool tmpId, bool publicOnly, int indentation) {
	int i;

	// Affiche la branche
	out << endl;

	switch(getDonneeType()) {
	case DONNEE_DEFAULT:
		out << "+err_d";	// Une donnée ne devrait jamais être de ce type, elle est créée avec pour recevoir automatiquement son vrai type
		break;
	case DONNEE_PUBLIC:
		out << "+pub";
		break;
	case DONNEE_PRIVATE:
		out << "+pri";
		break;
	case DONNEE_PRIVATE_SUB:
		out << "+prs";
		break;
	case DONNEE_LOCAL:
		out << "+loc";
		break;
	default:
		out << "+err";
		break;
	}

	out << "\t";

	for(i = 0 ; i < indentation ; i++) {
		out << "+";
	}

	out << "+ [id=" << _brancheId;
	if(tmpId) {
		out << " tmpId=" << _brancheTmpId;
	}
	out << " rv=" << getRevision() << "]";

	out << " '" << _brancheName << "'";

	if(!publicOnly || getDonneeType() != DONNEE_PRIVATE) {
		// Affiche les valeurs de la branche de manière ordonnée
		vector<Valeur*> valeurs = _valeurs;
		sort(valeurs.begin(), valeurs.end(), Valeur::highestId);

		for(Valeur* valeur : valeurs) {
			out << endl;

			switch(valeur->getDonneeType()) {
			case DONNEE_DEFAULT:
				out << ".err_d";	// Une donnée ne devrait jamais être de ce type, elle est créée avec pour recevoir automatiquement son vrai type
				break;
			case DONNEE_PUBLIC:
				out << ".pub";
				break;
			case DONNEE_PRIVATE:
				out << ".pri";
				break;
			case DONNEE_PRIVATE_SUB:
				out << ".prs";
				break;
			case DONNEE_LOCAL:
				out << ".loc";
				break;
			default:
				out << ".err";
				break;
			}

			out << "\t";

			for(i = 0 ; i < indentation+1 ; i++) {
				out << ".";
			}

			out << ". [id=" << valeur->getValeurId();
			if(tmpId) {
				out << " tmpId=" << valeur->getValeurTmpId();
			}

			out << " rv=" << valeur->getRevision() << "]";

			out << " '" << valeur->getValeurName() << "'";

			out << " <" << valeur->toString() << ">";
		}

		// Affiche les branches de la branche
		vector<Branche*> branches = _subBranches;
		sort(branches.begin(), branches.end(), Branche::highestId);

		for(Branche* br : branches) {
			br->print(out, distant, tmpId, publicOnly, indentation+1);
		}
	}
}

bool Branche::highestId(const Branche* left, const Branche* right) {
	int leftId = left->_brancheId;
	int rightId = right->_brancheId;

	if(leftId > 0 || rightId > 0) {
		return leftId < rightId;
	}
	else {
		return left->_brancheTmpId < right->_brancheTmpId;
	}
}
