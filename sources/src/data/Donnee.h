/*
 * Donnee.h
 *
 *  Created on: 9 mars 2013
 *      Author: vgdj7997
 */

#ifndef DONNEE_H_
#define DONNEE_H_

#include <map>

class DistantTreeProxy;
class MarqueurDistant;
class Branche;
class Valeur;

/**
 * Indique de quelle manière la données est partagée entre le serveur et les clients.
 * Si la donnée est une branche alors tous ses fils hériteront de son type.
 *
 * DONNEE_SERVER : Donnée partagée par le serveur avec tous les clients
 * DONNEE_CLIENT : Donnée partagée par le serveur avec chacun des clients (chaque client possède ses propres données)
 * DONNEE_LOCAL  : Donnée non-partagée, elle reste en local sur le serveur ou le client
 */
enum DONNEE_TYPE {
	DONNEE_DEFAULT,		// On peut créer une donnée de type
	DONNEE_SERVER,		// Donnée partagée par le serveur avec tous les clients
	DONNEE_CLIENT,		// Donnée partagée par le serveur avec chacun des clients (chaque client possède ses propres données)
	DONNEE_CLIENT_SUB,	// Donnée partagée par le serveur avec chacun des clients (chaque client possède ses propres données)
	DONNEE_LOCAL		// Donnée non-partagée, elle reste en local sur le serveur ou le client
};

class Donnee {
	/**
	 * Revision number of the data, incremented each time the data changes.
	 */
	int _revision;
	std::map<DistantTreeProxy*, MarqueurDistant*> _marqueurs;
	DONNEE_TYPE _donneeType;

protected:

	/**
	 * Indicates that the Donnee has changed by incrementing it's revision.
	 */
	void update();

public:
	Donnee(int revision, DONNEE_TYPE donneeType);
	virtual ~Donnee();

	MarqueurDistant* getMarqueur(DistantTreeProxy* distant);
	MarqueurDistant* addMarqueur(DistantTreeProxy* distant, int donneeTmpId);

	DONNEE_TYPE getDonneeType() const;

	/**
	 * Get the revision number of the data.
	 */
	int getRevision() const;
	void setRevision(int revision);

	static bool highestRevision(const Donnee* left, const Donnee* right);
};

#endif /* DONNEE_H_ */
