/*
 * Donnee.h
 *
 *  Created on: 9 mars 2013
 *      Author: vgdj7997
 */

#ifndef DONNEE_H_
#define DONNEE_H_

#include <map>

class DataTree;
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
	DONNEE_DEFAULT,		// Une donnée peut être créée de type par défaut pour recevoir le type approprié automatiquement
	DONNEE_PUBLIC,		// Donnée partagée par le serveur avec tous les clients
	DONNEE_PRIVATE,		// Donnée partagée par le serveur avec chacun des clients (chaque client possède ses propres données)
	DONNEE_PRIVATE_SUB,	// Donnée partagée par le serveur avec chacun des clients (chaque client possède ses propres données)
	DONNEE_LOCAL		// Donnée non-partagée, elle reste en local sur le serveur ou le client
};

enum UPDATE_MODE {
	ANY = 0,		// Server as well as any client can change the value
	SERVER_ONLY	// Server only can change the value, if client changes it it's value will not be shared with server or other clients and will be updated by server
};

class Donnee {
	DataTree* _dataTree;
	/**
	 * Revision number of the data, incremented each time the data changes.
	 */
	int _revision;
	std::map<DistantTreeProxy*, MarqueurDistant*> _marqueurs;
	DONNEE_TYPE _donneeType;
	UPDATE_MODE _updateMode;

protected:

	/**
	 * Indicates that the Donnee has changed by incrementing it's revision.
	 */
	void update();

public:
	Donnee(DataTree* dataTree, int revision, DONNEE_TYPE donneeType, UPDATE_MODE updateMode);
	virtual ~Donnee();

	MarqueurDistant* getMarqueur(DistantTreeProxy* distant);
	MarqueurDistant* addMarqueur(DistantTreeProxy* distant, int donneeTmpId);

	DataTree* getDataTree() const;
	DONNEE_TYPE getDonneeType() const;
	UPDATE_MODE getUpdateMode() const;

	/**
	 * Get the revision number of the data.
	 */
	int getRevision() const;
	void setRevision(int revision);

	static bool highestRevision(const Donnee* left, const Donnee* right);
};

#endif /* DONNEE_H_ */
