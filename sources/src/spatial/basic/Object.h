/*
 * Object.h
 *
 *  Created on: 29 juil. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_SPATIAL_BASIC_OBJECT_H_
#define SRC_SPATIAL_BASIC_OBJECT_H_

#include <string>

#include "tinyxml.h"

#include "spatial/MapLogger.h"
#include "util/GenRef.h"
#include "util/Erreur.h"

#include "spatial/basic/Object.h"

namespace jkt {

class CMap;

class Object {
	static jkt::CGenRef GENREF;

	std::string _name;				// Nom donn� � au g�o
	CMap* _map;						// Navigabilit� vers l'objet parent

protected:
	int _id;
	int _reference;					// Secondary / external identifier, can be a doublon
	std::string _tostring;

public:
	Object(CMap* map);
	Object(const Object& other);
	virtual ~Object();
	virtual void init() throw(CErreur);

	// Parent
	CMap* getMap();
	void setMap(CMap* map);

	// Gestion identiant, nom et r�f�rence
	void setName(const std::string &nom);		// Impl�mente le nom du g�o
	const char *getName();						// Renvoie le nom de l'objet
	virtual const char* toString();				// Description r�sum�e de l'objet
	unsigned int getId() const;

	// S�rialisation
	virtual bool Lit(TiXmlElement* el, MapLogger* mapLogger) = 0;
	virtual bool Save(TiXmlElement* element) = 0;		// Sauve l'objet g�o dans un fichier Map
};

} /* namespace jkt */

#endif /* SRC_SPATIAL_BASIC_OBJECT_H_ */
