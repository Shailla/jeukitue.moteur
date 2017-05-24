#ifndef __JKT__MATERIALTEXTURE_H
#define __JKT__MATERIALTEXTURE_H

#include <string>

#include "spatial/materiau/Material.h"
#include "util/Erreur.h"

namespace jkt
{
class Texture;

class CMaterialTexture : public CMaterial
{
public:
	Texture* _texture;	// ID de la texture fourni par openGL

	std::string m_FichierTexture;		// Nom du fichier de la texture associée

		// Constructeurs / Destructeurs
	CMaterialTexture(CMap* map);
	CMaterialTexture(CMap* map, const CMaterial &mat);
	~CMaterialTexture();

	virtual MapObject* clone() override;
	virtual void init() throw(CErreur) override;

		// Fonctions membres
	void initGL() throw(jkt::CErreur) override; // Lit le fichier de texture associé au matériau
	void freeGL() override;
	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) override;
	bool LitFichier(CMap* map, CIfstreamMap &fichier) override;
	bool SaveFichierMap(std::ofstream &fichier) override;
	bool Save(TiXmlElement* element) override;
	void Active() override;
	void Desactive() override;
};

}	// JktMoteur

#endif

