/*
 * Object.h
 *
 *  Created on: 29 juil. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_SPATIAL_BASIC_MAPOBJECT_H_
#define SRC_SPATIAL_BASIC_MAPOBJECT_H_

#include <string>

#include "tinyxml.h"

#include "spatial/MapLogger.h"
#include "spatial/basic/Object.h"
#include "spatial/basic/Drawable.h"
#include "spatial/basic/Geometrical.h"
#include "spatial/basic/Refreshable.h"
#include "spatial/basic/SolidAndTargettable.h"
#include "util/GenRef.h"
#include "util/Erreur.h"

#include "spatial/basic/Object.h"

namespace jkt {

class CMap;

class MapObject : public Object, public Drawable, public Geometrical, public Refreshable, public SolidAndTargettable {

public:
	MapObject(CMap* map);
	MapObject(const MapObject& other);
	virtual ~MapObject();


	/* ********************************************
	 * Manipulation de l'objet
	 * *******************************************/

	virtual MapObject* clone() = 0 ;
	virtual void init() throw(CErreur) override = 0;


	/* ********************************************
	 * Serialisation de l'objet
	 * *******************************************/

	virtual bool Lit(TiXmlElement* el, MapLogger* mapLogger) override = 0;	// Lit l'objet depuis un fichier Map
	virtual bool Save(TiXmlElement* element) override = 0;					// Sauve l'objet dans un fichier Map


	/* ********************************************
	 * Manipulation de l'évolution de l'objet
	 * *******************************************/

	virtual void refresh( CGame *game ) override;


	/* ********************************************
	 * Affichage de l'objet
	 * *******************************************/

	virtual void Affiche() override;												// Affiche l'objet géométrique
	virtual void AfficheSelection(float r,float v,float b) override;				// Affiche l'objet géométrique en couleur unique
	virtual void initGL() override;												// Initialisation de l'objet géométrique
	virtual void freeGL() override;												// Libération des ressources de l'objet dans le contexte OpenGL


	/* ********************************************
	 * Manipulation géométrique de l'objet
	 * *******************************************/

	virtual void EchangeXY() override;										// Echange les coordonnées X et Y de l'objet
	virtual void EchangeXZ() override;										// Echange les coordonnées X et Z de l'objet
	virtual void EchangeYZ() override;										// Echange les coordonnées Y et Z de l'objet
	virtual void Scale(float scaleX, float scaleY, float scaleZ) override;	// Homothétie pondérée selon X, Y et Z de l'objet
	virtual void translate(float x, float y, float z) override;				// Translation pondérée selon X, Y et Z de l'objet


	/* ********************************************
	 * Gestion des contacts de l'objet
	 * *******************************************/

	virtual void GereContactPlayer(float positionPlayer[3], CPlayer *player) override;	// Gère les contact entre 'player' et l'objet géo
	virtual float GereLaserPlayer(float pos[3], CV3D& Dir, float dist) override;			// Voir la définition de la fonction
	virtual bool Contact(const float pos[3], float dist) override;
};

} /* namespace jkt */

#endif /* SRC_SPATIAL_BASIC_MAPOBJECT_H_ */
