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
#include "spatial/MapLogger.h"

namespace JktMoteur {

class EntryPoint {
	JktUtils::CV3D _entryPosition;	// Position du point d'entr�e
public:
	EntryPoint();
	EntryPoint(const float entryPosition[3]);
	virtual ~EntryPoint();

	// M�thodes de s�rialisation
	bool Lit(TiXmlElement* element, MapLogger* mapLogger);		// Lit le point d'entr�e joueur d'un fichier Map format XML
	bool Save(TiXmlElement* element);							// Lit le point d'entr�e joueur dans un fichier Map format XML

	// M�thodes de manipulation g�om�trique
	void EchangeXY();										// Echange les coordonn�es X et Y de l'objet
	void EchangeXZ();										// Echange les coordonn�es X et Z de l'objet
	void EchangeYZ();										// Echange les coordonn�es Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ);	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
	void translate(float x, float y, float z);				// Translation pond�r�e selon X, Y et Z de l'objet

	JktUtils::CV3D getEntryPosition();
};

} /* namespace JktLua */
#endif /* ENTRYPOINT_H_ */
