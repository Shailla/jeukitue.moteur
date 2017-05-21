#ifndef __JKT__MOTEURPARTICULES_H
#define __JKT__MOTEURPARTICULES_H

#include "spatial/basic/MapObject.h"

namespace jkt
{

class CMoteurParticules : public MapObject {
public:
		// Fonctions membres
private:

public:
		// Constructeurs
	CMoteurParticules(CMap* parent);
	virtual ~CMoteurParticules(void);

	virtual MapObject* clone() = 0;
	virtual void init() throw(CErreur) = 0;
	virtual bool Lit(TiXmlElement* el, MapLogger* mapLogger) override = 0;	// Lit l'objet depuis un fichier Map
	virtual bool Save(TiXmlElement* element) override = 0;					// Sauve l'objet dans un fichier Map
};

}	// JktMoteur

#endif
