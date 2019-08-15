#ifndef __JKT__MATERIALTREE_H
#define __JKT__MATERIALTREE_H

#include <iostream>
#include <string>

#include "spatial/materiau/MaterialTexture.h"

#include "spatial/materiau/Material.h"

namespace jkt
{

class CMaterialMulti:public CMaterial
{
	int m_NbrTex;				// Nombre de sous-matériaux

public:
	CMaterial** m_TabMat;		// Tableau des sous-matériaux

	// Constructeurs / Destructeurs
	CMaterialMulti(CMap* map);
	~CMaterialMulti();

	virtual MapObject* clone() override;
	virtual void init() noexcept(false) override;

	// Fonctions membres
	void initGL() override;
	void freeGL() override;
	void NbrTex(int nbr);		// Renseigne le nombre de sous-matériaux
	int NbrTex();				// Renvoie le nombre de sous-matériaux
	CMaterial* getMat(int i);	// Renvoie le sous-matériau d'index i
	bool Save(TiXmlElement* element) override;
	bool Lit(TiXmlElement* element, CMap& map, MapLogger* mapLogger) noexcept(false) override;
	const char* toString();
};

}	// JktMoteur

#endif
