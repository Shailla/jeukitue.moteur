/*
 * EntryPoint.h
 *
 *  Created on: 6 mai 2012
 *      Author: Erwin
 */

#ifndef ENTRYPOINT_H_
#define ENTRYPOINT_H_

#include "tinyxml.h"

#include "util/V3D.h"
#include "spatial/basic/MapObject.h"
#include "spatial/MapLogger.h"

namespace jkt {

class EntryPoint : public MapObject {
	jkt::CV3D _entryPosition;	// Position du point d'entrée
public:
	EntryPoint(CMap* map);
	EntryPoint(CMap* map, const float entryPosition[3]);
	virtual ~EntryPoint();

	MapObject* clone() override;
	void init() throw(CErreur) override;

	// Méthodes de sérialisation
	bool Lit(TiXmlElement* element, MapLogger* mapLogger) override;		// Lit le point d'entrée joueur d'un fichier Map format XML
	bool Save(TiXmlElement* element) override;							// Lit le point d'entrée joueur dans un fichier Map format XML

	// Méthodes de manipulation géométrique
	void EchangeXY() override;										// Echange les coordonnées X et Y de l'objet
	void EchangeXZ() override;										// Echange les coordonnées X et Z de l'objet
	void EchangeYZ() override;										// Echange les coordonnées Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ) override;	// Homothétie pondérée selon X, Y et Z de l'objet
	void translate(float x, float y, float z) override;				// Translation pondérée selon X, Y et Z de l'objet

	jkt::CV3D getEntryPosition();
};

} /* namespace jkt */
#endif /* ENTRYPOINT_H_ */
