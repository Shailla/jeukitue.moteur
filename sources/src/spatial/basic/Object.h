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

	int _id;				// Identifiant unique de l'object
	std::string _name;		// Nom de l'objet (pas forcément unique)
	std::string _type;
	CMap* _parent;			// Navigabilité vers la Map d'appartenance
	bool _abstract;

protected:
	std::string _tostring;
	std::string _reference;					// Secondary / external identifier, can be a doublon

public:
	Object(CMap* map, const std::string& type);
	Object(const Object& other);
	virtual ~Object();
	virtual void init() noexcept(false);

	bool isAbstract() const;
	void setAbstract(bool abstract);

	// Parent
	CMap* getMap();
	void setMap(CMap* map);

	// Gestion identiant, nom et référence
	void setName(const std::string &nom);		// Impl�mente le nom du g�o
	const char *getName();						// Renvoie le nom de l'objet
	virtual const char* toString();				// Description r�sum�e de l'objet
	int getId() const;
	std::string getReference() const;
	const std::string& getType() const;

	// Sérialisation
	virtual bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) noexcept(false) = 0;
	virtual bool Save(TiXmlElement* element) noexcept(false) = 0;								// Sauve l'objet g�o dans un fichier Map
};

} /* namespace jkt */

#endif /* SRC_SPATIAL_BASIC_OBJECT_H_ */
