#include "TexVertexList.h"


namespace JktMoteur
{

CTexVertexList::CTexVertex::CTexVertex(float* tvertex, int nbr) 
{
	m_TabTexVertex = tvertex;
	m_NumTexVertex = nbr;
}

CTexVertexList::CTexVertex::~CTexVertex() 
{
	if(m_TabTexVertex)
		delete[] m_TabTexVertex;
}

float* CTexVertexList::CTexVertex::getTexVertex() const
{
	return m_TabTexVertex;
}

int CTexVertexList::CTexVertex::getNbrTexVertex() const
{
	return m_NumTexVertex;
}

CTexVertexList::CTexVertexList(int nbrListe)
{
	m_ListeTexVertex = new CTexVertex*[nbrListe];
	for(int canal=0 ; canal<nbrListe ; canal++)
		m_ListeTexVertex[canal] = 0;

	m_NbrListes = nbrListe;
}

CTexVertexList::~CTexVertexList()
{
	if(m_ListeTexVertex)
	{
		for(int i=0 ; i<m_NbrListes ; i++)
		{
			if(m_ListeTexVertex[i])
				delete m_ListeTexVertex[i];
		}


		delete[] m_ListeTexVertex;
	}
}

float* CTexVertexList::getTexVertex(int canal) const
{
	if(m_ListeTexVertex[canal])
		return m_ListeTexVertex[canal]->getTexVertex();
	else
		return 0;
}

int CTexVertexList::getNbrCanaux() const
{
	return m_NbrListes;
}

int CTexVertexList::getNbrTexVertex(int canal) const
{
	return m_ListeTexVertex[canal]->getNbrTexVertex();
}
void CTexVertexList::setTexVertex(int canal, float* tvertex, int nbrTexVertex)
{
	if(m_ListeTexVertex[canal])
		delete m_ListeTexVertex[canal];

	m_ListeTexVertex[canal] = new CTexVertex(tvertex, nbrTexVertex);
}

}
