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
	jkt::CV3D _entryPosition;	// Position du point d'entr�e
public:
	EntryPoint(CMap* map);
	EntryPoint(CMap* map, const float entryPosition[3]);
	virtual ~EntryPoint();

	MapObject* clone() override;
	void init() throw(CErreur) override;

	// Méthodes de sérialisation
	bool Lit(TiXmlElement* element, CMap& map, MapLogger* mapLogger) throw(CErreur) override;		// Lit le point d'entr�e joueur d'un fichier Map format XML
	bool Save(TiXmlElement* element) throw(CErreur) override;							// Lit le point d'entr�e joueur dans un fichier Map format XML

	// M�thodes de manipulation g�om�trique
	void EchangeXY() override;										// Echange les coordonn�es X et Y de l'objet
	void EchangeXZ() override;										// Echange les coordonn�es X et Z de l'objet
	void EchangeYZ() override;										// Echange les coordonn�es Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ) override;	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
	void translate(float x, float y, float z) override;				// Translation pond�r�e selon X, Y et Z de l'objet

	jkt::CV3D getEntryPosition();
};

} /* namespace jkt */
#endif /* ENTRYPOINT_H_ */
