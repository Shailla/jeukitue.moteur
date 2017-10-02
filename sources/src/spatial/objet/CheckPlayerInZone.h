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
	// Identifiant du check, permet � un plugin par exemple de savoir quel check a �t� activ�
	std::string _id;

	// Permet d'activer l'affichage dans la Map de la zone de d�tection (� fin de debug essentiellement)
	bool _isVisible;

	// Indique que le joueur est actuellement d�tect� dans la zone
	bool _playerInZone;
	std::set<CPlayer*> _playersInZone;

	// Zone dans laquelle un joueur d�clenche la d�tection
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
	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) throw(CErreur) override;
	bool Save(TiXmlElement* element) throw(CErreur) override;

	// Geometrical
	void EchangeXY() override;										// Echange les coordonn�es X et Y de l'objet
	void EchangeXZ() override;										// Echange les coordonn�es X et Z de l'objet
	void EchangeYZ() override;										// Echange les coordonn�es Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ) override;	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
	void translate( float x, float y, float z ) override;			// Translation pond�r�e selon X, Y et Z de l'objet

	// Drawable
	void initGL() override;
	void freeGL() override;

	void Affiche() override;											// Affiche l'objet g�om�trique
	void AfficheHighlighted(float r,float v,float b) override;

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
