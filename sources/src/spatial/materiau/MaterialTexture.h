#ifndef __JKT__MATERIALTEXTURE_H
#define __JKT__MATERIALTEXTURE_H

#include <iostream>
#include <string>

using namespace std;

#include "spatial/materiau/Material.h"
#include "util/Erreur.h"

namespace JktMoteur
{
class Texture;

class CMaterialTexture:public CMaterial
{
public:
	Texture* _texture;	// ID de la texture fourni par openGL

	std::string m_FichierTexture;		// Nom du fichier de la texture associée

		// Constructeurs / Destructeurs
	CMaterialTexture();
	CMaterialTexture( const CMaterial &mat );
	~CMaterialTexture();

		// Fonctions membres
	void initGL() throw(JktUtils::CErreur); // Lit le fichier de texture associé au matériau
	bool Lit(TiXmlElement* el, string &repertoire);
	bool LitFichier(CIfstreamMap &fichier);
	bool SaveFichierMap(std::ofstream &fichier);
	bool Save(TiXmlElement* element);
	void Active();
	void Desactive();
};

}	// JktMoteur

#endif

