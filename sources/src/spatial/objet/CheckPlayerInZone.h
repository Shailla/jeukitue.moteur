/*
 * CheckPlayerInZone.h
 *
 *  Created on: 24 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_
#define SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_

#include <set>

#include "util/V3D.h"
#include "spatial/basic/MapObject.h"

namespace jkt
{

class CheckPlayerInZone : public MapObject {
	// Identifiant du check, permet à un plugin par exemple de savoir quel check a été activé
	std::string _id;

	// Permet d'activer l'affichage dans la Map de la zone de détection (à fin de debug essentiellement)
	bool _isVisible;

	// Indique que le joueur est actuellement détecté dans la zone
	bool _playerInZone;
	std::set<CPlayer*> _playersInZone;

	// Zone dans laquelle un joueur déclenche la détection
	float _zoneDetectionXmin;
	float _zoneDetectionXmax;
	float _zoneDetectionYmin;
	float _zoneDetectionYmax;
	float _zoneDetectionZmin;
	float _zoneDetectionZmax;

	bool checkPositionInZone(float position[3]);

public:
	static const char* identifier;

	CheckPlayerInZone(CMap* map, const std::string& id, float zoneDetectionXmin, float zoneDetectionXmax, float zoneDetectionYmin, float zoneDetectionYmax, float zoneDetectionZmin, float zoneDetectionZmax);
	CheckPlayerInZone(CMap* map, const CheckPlayerInZone& other);
	virtual ~CheckPlayerInZone();

	MapObject* clone() override;

	// Object
	void init() throw(CErreur) override;								// Initialisation de l'objet
	bool Lit(TiXmlElement* element, MapLogger* mapLogger) override;
	bool Save(TiXmlElement* element) override;

	// Geometrical
	void EchangeXY() override;										// Echange les coordonnées X et Y de l'objet
	void EchangeXZ() override;										// Echange les coordonnées X et Z de l'objet
	void EchangeYZ() override;										// Echange les coordonnées Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ) override;	// Homothétie pondérée selon X, Y et Z de l'objet
	void translate( float x, float y, float z ) override;			// Translation pondérée selon X, Y et Z de l'objet

	// Drawable
	void initGL() override;
	void freeGL() override;

	void Affiche() override;											// Affiche l'objet géométrique
	void AfficheSelection(float r,float v,float b) override;

	// Refreshable
	void beginRefresh() override;
	void refresh(CGame* game) override;
	void endRefresh() override;

	void setVisibility(bool isVisible);
	bool isPlayerInZone() const;

	const char* toString();
};

}

#endif /* SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_ */
