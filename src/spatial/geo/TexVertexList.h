#pragma once

/**
	Liste des tableaux de sommets de textures d'un objet
**/

namespace JKT_PACKAGE_MOTEUR3D
{

class CTexVertexList
{
	/**
		Tableau de sommets de texture
	**/
	class CTexVertex
	{
		int m_NumTexVertex;		// Nombre de sommets de texture
		float* m_TabTexVertex;	// Sommets de texture
	public:
		CTexVertex(float* tvertex, int nbr);
		~CTexVertex();

		float* getTexVertex() const;
		int getNbrTexVertex() const;
	};

	int m_NbrListes;				// Nombre de tableaux de sommets de textures de la liste
	CTexVertex** m_ListeTexVertex;	// Liste des tableau de sommets de textures
public:
	CTexVertexList(int nbrListe);	// Construit une liste de 'nbrListe' tableaux de sommets de texture
	~CTexVertexList();

	int getNbrCanaux() const;				// Retourne le nombre de listes (=nbre de canaux)
	int getNbrTexVertex(int canal) const;
	float* getTexVertex(int canal) const;	// Retourne le tableau du canal demandé
	void setTexVertex(int canal, float* tvertex, int nbrTexVertex);	//
};

}
