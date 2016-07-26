/*
 * CheckPlayerInZone.h
 *
 *  Created on: 24 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_
#define SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_

#include <string>

#include "spatial/geo/Geo.h"

namespace jkt
{

class CheckPlayerInZone : public CGeo {
	std::string tostring;

	// Identifiant du check, permet � un plugin par exemple de savoir quel check a �t� activ�
	std::string _id;

	// Permet d'activer l'affichage dans la Map de la zone de d�tection (� fin de debug essentiellement)
	bool _isVisible;

	// Zone dans laquelle un joueur d�clenche la d�tection
	float _zoneDetectionXmin;
	float _zoneDetectionXmax;
	float _zoneDetectionYmin;
	float _zoneDetectionYmax;
	float _zoneDetectionZmin;
	float _zoneDetectionZmax;
public:
	static const char* identifier;

	CheckPlayerInZone(CMap* map, const std::string& id, float zoneDetectionXmin, float zoneDetectionXmax, float zoneDetectionYmin, float zoneDetectionYmax, float zoneDetectionZmin, float zoneDetectionZmax);
	CheckPlayerInZone(CMap* map, const CheckPlayerInZone& other);
	virtual ~CheckPlayerInZone();
	CGeo* clone() override;

	void Init() override;								// Initialisation de l'objet
	void initGL() override;
	void freeGL() override;

	void EchangeXY() override;										// Echange les coordonn�es X et Y de l'objet
	void EchangeXZ() override;										// Echange les coordonn�es X et Z de l'objet
	void EchangeYZ() override;										// Echange les coordonn�es Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ) override;	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
	void translate( float x, float y, float z ) override;			// Translation pond�r�e selon X, Y et Z de l'objet

	bool Lit(TiXmlElement* element, MapLogger* mapLogger) override;
	bool Save(TiXmlElement* element) override;

	void Affiche() override;											// Affiche l'objet g�om�trique
	void AfficheSelection(float r,float v,float b) override;

	void GereContactPlayer(float positionPlayer[3], CPlayer *player) override;
	float GereLaserPlayer(float pos[3], CV3D &Dir, float dist) override;		// Voir la d�finition de la fonction

	void setVisibility(bool isVisible);
	const char* toString();
};

}

#endif /* SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_ */
