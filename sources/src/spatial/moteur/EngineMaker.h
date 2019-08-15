#pragma warning( disable : 4290 )	// Evite les warnings sur les d�claration de throw

#ifndef __JKT__ENGINEMAKER_H
#define __JKT__ENGINEMAKER_H

/**	Cette classe a pour r�le de servir de r�ceptable des donn�es concernant un
	nouvel objet g�om�trique, puis une fois toutes les donn�es disponibles,
	elle sert surtout � cr�er un objet g�om�trique (CGeo) optimis� pour
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
	static CMoteurParticules* Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) noexcept(false);
};

}	// JktMoteur

#endif	// __JKT__ENGINEMAKER_H

