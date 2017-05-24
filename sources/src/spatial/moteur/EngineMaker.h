#pragma warning( disable : 4290 )	// Evite les warnings sur les déclaration de throw

#ifndef __JKT__ENGINEMAKER_H
#define __JKT__ENGINEMAKER_H

/**	Cette classe a pour rôle de servir de réceptable des données concernant un
	nouvel objet géométrique, puis une fois toutes les données disponibles,
	elle sert surtout à créer un objet géométrique (CGeo) optimisé pour
	le type d'affichage de l'objet (avec ou sans texture, indexation,...).
*/

#include <map>
#include <fstream>

#include "tinyxml.h"

#include "util/Erreur.h"
#include "spatial/MapLogger.h"

#include "spatial/basic/MapObject.h"

namespace jkt
{
class CMap;
class CMoteurParticules;

class EngineMaker
{
public:
	static CMoteurParticules* Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) throw(CErreur);
};

}	// JktMoteur

#endif	// __JKT__ENGINEMAKER_H

