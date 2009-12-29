
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "util/GenRef.h"
#include "IfstreamMap.h"

#include "Geo.h"

namespace JKT_PACKAGE_MOTEUR3D
{
JKT_PACKAGE_UTILS::CGenRef CGeo::GENREF;

CGeo::CGeo( CMap *map )
{
	pMap = map;
	m_Reference = GENREF.genRef();	// Génère une référence pour l'objet
}

CGeo::~CGeo()
{
	GENREF.delRef(m_Reference);		// Libère la référence de l'objet
}

CMap *CGeo::getMap()
{	return pMap;	}

unsigned int CGeo::getReference()
{
	return m_Reference;
}

/*bool CGeo::SaveFichierMap(ofstream &fichier)
{
	fichier << "\tReference\t\t" << getReference();
	fichier << "\tNom\t\t\t\t\"" << getName() << '\"';	// Sauvegarde du nom de l'objet

	return true;
}*/

/*bool CGeo::LitFichier(CIfstreamMap &fichier)
{
	string mot, nom;
	char ch;

		// Lecture de la référence de l'object
	fichier >> mot;
	if( mot!="Reference" )
	{
		cerr << __LINE__;
		return false;
	}
	unsigned int reference;
	fichier >> reference;		// A FAIRE : comment utiliser la référence (gestion des doublons...)


		// Lecture du nom de l'objet géo entre guillemets
	fichier >> mot;
	if( mot!="Nom" )
	{
		cerr << __LINE__;
		return false;
	}

	fichier >> ch;		// Recherche du guillemet ouvrant
	if( ch!='\"' )
	{
		cerr << __LINE__;
		return false;
	}

	do
	{
		fichier.get( ch );
		if( ch=='\n' )
		{
			cerr << __LINE__;
			return false;
		}

		if( ch!='\"' )
			nom += ch;
	}while( ch!='\"' );

	setName( nom );

	return true;
}*/

void CGeo::setName( const string &nom )
{
	m_Nom.clear();
	m_Nom = nom;
}

const char *CGeo::getName()
{
	return m_Nom.c_str();
}

bool CGeo::Contact( const float pos[3], float dist )
{
	return false;
}

}	// JKT_PACKAGE_MOTEUR3D
