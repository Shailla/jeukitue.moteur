
#ifndef __JKT__MATERIAL_H
#define __JKT__MATERIAL_H

#include <iostream>
#include <string>

#include "tinyxml.h"

#include "spatial/MapLogger.h"
#include "spatial/basic/MapObject.h"

namespace jkt
{

class CIfstreamMap;

class CMaterial : public MapObject {
	friend class CMaterialMaker;
public:
	enum MAT_TYPE {
		MAT_TYPE_SIMPLE = 0,	// Mat�riau simple
		MAT_TYPE_TEXTURE,		// Mat�riau de type simple texture
		MAT_TYPE_MULTI,			// Mat�riau de type textures multiples
	};
	std::string tostring;
protected:
	MAT_TYPE m_Type;	// type de mat�riau : MAT_TYPE_TEXTURE = CMaterialTexture, MAT_TYPE_MULTI = textures multiples
	unsigned int m_Ref;

public:
	float m_Ambient[4];		// Composante de lumi�re ambiante de l'objet g�o
	float m_Diffuse[4];		// Composante de lumi�re diffuse de l'objet g�o
	float m_Specular[4];	// Composante de lumi�re sp�culaire de l'objet g�o

		// Constructeurs / Destructeur
	CMaterial(CMap* map);
	virtual ~CMaterial();

	virtual MapObject* clone() override;
	virtual void init() noexcept(false) override;

	MAT_TYPE Type() const;	// Retourne le type de mat�riau (simple, tree, ...)

	// S�rialisation
	virtual bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) noexcept(false) override;
	virtual bool Save(TiXmlElement* element) noexcept(false) override;

	virtual void initGL() {};	// Lit le fichier de texture associ� au mat�riau
	virtual void freeGL() {};	// Lit le fichier de texture associ� au mat�riau
	virtual void Active();
	virtual void Desactive();
	const char* toString();
	unsigned int getRef() const;	// Renvoie la r�f�rence du mat�riau
	void setRef(unsigned int ref);
};

}	// JktMoteur

#endif

