
#ifndef __JKT__CHANTEX_H
#define __JKT__CHANTEX_H

namespace JktMoteur 
{

class CChanTex
{
	unsigned int m_NumTexVertex;	// Nombre de sommets de texture
	unsigned int m_NumTexFaces;		// Nombre d'index de sommets de texture
	float *m_TabTexVertex;		// Sommets de texture
	int *m_TabTexFaces;			// Index des sommets de texture
public:
	CChanTex();
	~CChanTex();

	void setTexVertex(unsigned int nbr, float* texvertex);
	void setTexFaces(unsigned int nbr, int* texfaces);
	unsigned int getNumTexVertex();
	unsigned int getNumTexFaces();
	float* getTexVertex();
	int* getTexFaces();
};

}

#endif
