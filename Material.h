
#ifndef __JKT__MATERIAL_H
#define __JKT__MATERIAL_H

#include <iostream>
#include <string>
#include "tinyxml.h"

namespace JKT_PACKAGE_MOTEUR3D
{

class CIfstreamMap;

class CMaterial
{
	friend class CMaterialMaker;
public:
	enum MAT_TYPE {
		MAT_TYPE_SIMPLE = 0,	// Matériau simple
		MAT_TYPE_TEXTURE,		// Matériau de type simple texture
		MAT_TYPE_MULTI,			// Matériau de type textures multiples
	};
	std::string tostring;
protected:
	MAT_TYPE m_Type;	// type de matériau : MAT_TYPE_TEXTURE = CMaterialTexture, MAT_TYPE_MULTI = textures multiples
	unsigned int m_Ref;

public:
	float m_Ambient[4];		// Composante de lumière ambiante de l'objet géo
	float m_Diffuse[4];		// Composante de lumière diffuse de l'objet géo
	float m_Specular[4];	// Composante de lumière spéculaire de l'objet géo

		// Constructeurs / Destructeur
	CMaterial();
	virtual ~CMaterial();

	MAT_TYPE Type() const;	// Retourne le type de matériau (simple, tree, ...)

	virtual void LitTexture() {};	// Lit le fichier de texture associé au matériau
	virtual bool LitFichier(CIfstreamMap &fichier);
	virtual bool Lit(TiXmlElement* el, string &repertoire);
	virtual bool SaveFichierMap(std::ofstream &fichier);
	virtual bool Save(TiXmlElement* element);
	virtual void Active();
	virtual void Desactive();
	const char* toString();
	unsigned int getRef() const;	// Renvoie la référence du matériau
	void setRef(unsigned int ref);
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

