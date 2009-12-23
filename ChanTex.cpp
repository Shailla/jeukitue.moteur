
#ifndef __JKT__CHANTEX_CPP
#define __JKT__CHANTEX_CPP

#include "ChanTex.h"

namespace JKT_PACKAGE_MOTEUR3D
{

CChanTex::CChanTex() {
	m_NumTexVertex = 0;
	m_NumTexFaces = 0;
	m_TabTexVertex = 0;
	m_TabTexFaces = 0;
}

CChanTex::~CChanTex() {
	if( m_TabTexVertex )
		delete[] m_TabTexVertex;
	if( m_TabTexFaces )
		delete[] m_TabTexFaces;
}

void CChanTex::setTexVertex(unsigned int nbr, float* texvertex)
{
	m_NumTexVertex = nbr;

	if(m_TabTexVertex)
		delete[] m_TabTexVertex;
	m_TabTexVertex = texvertex;
}

void CChanTex::setTexFaces(unsigned int nbr, int* texfaces)
{
	m_NumTexFaces = nbr;

	if(m_TabTexFaces)
		delete[] m_TabTexFaces;
	m_TabTexFaces = texfaces;
}

unsigned int CChanTex::getNumTexVertex()
{
	return m_NumTexVertex;
}

unsigned int CChanTex::getNumTexFaces()
{
	return m_NumTexFaces;
}

float* CChanTex::getTexVertex()
{
	return m_TabTexVertex;
}

int* CChanTex::getTexFaces()
{
	return m_TabTexFaces;
}

}

#endif
