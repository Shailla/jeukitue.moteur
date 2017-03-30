#ifndef __JKT__MATERIALTEXTURE_H
#define __JKT__MATERIALTEXTURE_H

#include <string>

#include "spatial/materiau/Material.h"
#include "util/Erreur.h"

namespace jkt
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
	void initGL() throw(jkt::CErreur) override; // Lit le fichier de texture associé au matériau
	void freeGL() override;
	bool Lit(TiXmlElement* el, std::string &repertoire, MapLogger* mapLogger) override;
	bool LitFichier(CIfstreamMap &fichier) override;
	bool SaveFichierMap(std::ofstream &fichier) override;
	bool Save(TiXmlElement* element) override;
	void Active() override;
	void Desactive() override;
};

}	// JktMoteur

#endif

