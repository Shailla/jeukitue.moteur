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

namespace JktMoteur {

class EntryPoint {
	JktUtils::CV3D _entryPosition;	// Position du point d'entrée
public:
	EntryPoint();
	EntryPoint(const float entryPosition[3]);
	virtual ~EntryPoint();

	// Méthodes de sérialisation
	bool Lit(TiXmlElement* element);				// Lit le point d'entrée joueur d'un fichier Map format XML
	bool Save(TiXmlElement* element);				// Lit le point d'entrée joueur dans un fichier Map format XML

	// Méthodes de manipulation géométrique
	void EchangeXY();										// Echange les coordonnées X et Y de l'objet
	void EchangeXZ();										// Echange les coordonnées X et Z de l'objet
	void EchangeYZ();										// Echange les coordonnées Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ);	// Homothétie pondérée selon X, Y et Z de l'objet
	void translate(float x, float y, float z);				// Translation pondérée selon X, Y et Z de l'objet

	JktUtils::CV3D getEntryPosition();
};

} /* namespace JktLua */
#endif /* ENTRYPOINT_H_ */
