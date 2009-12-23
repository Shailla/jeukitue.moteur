
#ifndef __JKT__CLIGHTOMNI_H
#define __JKT__CLIGHTOMNI_H

#include <iostream>

#include "tinyxml.h"

#include "Light.h"

namespace JKT_PACKAGE_MOTEUR3D
{

class CLightOmni : public CLight
{
public:
	CLightOmni();
	~CLightOmni();

	bool Lit(TiXmlElement* element);
	bool LitFichier(CIfstreamMap &fichier);
	bool SaveFichierMap(std::ofstream &fichier);
	bool Save(TiXmlElement* element);

	void Active();
	void Desactive();
};

}	// JKT_PACKAGE_MOTEUR3D

#endif
