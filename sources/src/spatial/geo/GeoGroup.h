/*
 * GeoGroup.h
 *
 *  Created on: 4 oct. 2017
 *      Author: Erwin
 */

#ifndef SRC_SPATIAL_GEO_GEOGROUP_H_
#define SRC_SPATIAL_GEO_GEOGROUP_H_

#include <vector>

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/GL.H>

#include "util/Erreur.h"
#include "spatial/basic/MapObject.h"

namespace jkt {

class GeoGroup : public MapObject {
	std::vector<MapObject*> _geos;
	float* _transformation;
	bool _solid;								// Indique si l'objet est solide ou non

public:
	static const char* identifier;

	GeoGroup(CMap* map);
	GeoGroup(const GeoGroup& other);
	virtual ~GeoGroup();

	friend class GeoMaker;
	std::string tostring;

	MapObject* clone() override;

	void init() throw(CErreur) override;								// Initialisation de l'objet
	void initGL() override;
	void freeGL() override;

	bool checkContact( const float pos[3], float dist ) override;
	void gereContactPlayer(float positionPlayer[3], CPlayer *player, float deltaTime) override;
	float gereLaserPlayer(float pos[3], CV3D &Dir, float dist) override;		// Voir la définition de la fonction

	// Fonctions pour l'interface CGeo
	bool Lit(TiXmlElement* element, CMap& map, MapLogger* mapLogger) throw(CErreur) override;
	bool Save(TiXmlElement* element) throw(CErreur) override;

	void affiche() override;									// Affiche l'objet géo
	void afficheHighlighted(float r,float v,float b) override;	// Affiche l'objet en couleur unique
	const char* toString() override;

	void setTransformation(const float transformation[16]);
};

} /* namespace jkt */

#endif /* SRC_SPATIAL_GEO_GEOGROUP_H_ */
