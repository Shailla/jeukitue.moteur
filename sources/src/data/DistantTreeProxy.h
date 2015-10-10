/*
 * DistantTreeProxy.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef DISTANTTREEPROXY_H_
#define DISTANTTREEPROXY_H_

#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "data/DistantTreeControl.h"

#include "reseau/new/Interlocutor2.h"

class Interlocutor;
class MarqueurDistant;
class Donnee;
class Changement;

class DistantTreeProxy {
	Interlocutor2* _interlocutor;
	std::map<Donnee*, MarqueurDistant*> _marqueurs;
	Uint32 _updateServerToClientTime;	// Datage du dernier update de l'arbre du serveur vers le client

	DistantTreeControl _control;

public:
	DistantTreeProxy(Interlocutor2* interlocutor);
	virtual ~DistantTreeProxy();

	DistantTreeControl& getControl();
	Interlocutor2* getInterlocutor() const;
	MarqueurDistant* getMarqueur(Donnee* donnee);
	std::map<Donnee*, MarqueurDistant*>& getMarqueurs();
	MarqueurDistant* addMarqueur(Donnee* donnee, int donneeTmpId);
	void collecteChangementsInClientTree(std::vector<Changement*>& changements);
	void collecteChangementsInServerTree(std::vector<Changement*>& changements);

	Uint32 getUpdateServerToClientTime() const;
	void setUpdateServerToClientTime(Uint32 updateServerToClientTime);
};

#endif /* DISTANTTREEPROXY_H_ */
