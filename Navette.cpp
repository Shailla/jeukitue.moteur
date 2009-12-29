
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <map>

using namespace std;

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>

class CGame;

#include "XmlVocabulaire.h"
#include "Erreur.h"
#include "V3D.h"
#include "IfstreamMap.h"
#include "GeoMaker.h"
#include "MaterialTexture.h"
#include "GeoObject.h"
#include "son/DemonSons.h"
#include "Game.h"

#include "Navette.h"

namespace JKT_PACKAGE_MOTEUR3D
{

const char* CNavette::identifier = "Navette";

CPointNavette::CPointNavette()
{
	m_Vitesse = 0.0f;
}

CPointNavette::CPointNavette( const CPointNavette& pp )
{
	m_Vitesse = pp.m_Vitesse;
	m_Position = pp.m_Position;
}

CPointNavette::~CPointNavette()
{
}

void CPointNavette::operator =( const CPointNavette &pp )
{
	m_Vitesse = pp.m_Vitesse;
	m_Position = pp.m_Position;
}

void CPointNavette::LitFichierPoint( CIfstreamMap &fichier )
{
	string mot;

	if( !( fichier>>mot ) )		// Lecture de la distance parcourue par la navette lors de son ouverture
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierNavette";
		throw CErreur( 0, erreur );
	}
	if( mot!="Vitesse" )
	{
		string erreur( "\nFichier corrompu (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierNavette";
		throw CErreur( 0, erreur );
	}
	if( !( fichier>>m_Vitesse ) )
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierNavette";
		throw CErreur( 0, erreur );
	}
	if( !( fichier>>mot ) )		// Lecture de la distance parcourue par la navette lors de son ouverture
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierNavette";
		throw CErreur( 0, erreur );
	}
	if( mot!="Position" )
	{
		string erreur( "\nFichier corrompu (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierNavette";
		throw CErreur( 0, erreur );
	}
	if( !( fichier>>m_Position.X ) )
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierNavette";
		throw CErreur( 0, erreur );
	}
	if( !( fichier>>m_Position.Y ) )
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierNavette";
		throw CErreur( 0, erreur );
	}
	if( !( fichier>>m_Position.Z ) )
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierNavette";
		throw CErreur( 0, erreur );
	}
}

void CPointNavette::SaveFichierPoint( ostream &fichier )
{
	fichier << "\n\tVitesse\t" << m_Vitesse;
	fichier << "\n\tPosition\t" << m_Position.X << "\t" << m_Position.Y << "\t" << m_Position.Z << endl;
}

void CPointNavette::SavePoint(TiXmlElement* element)
{
		// Sauve les données générales
	TiXmlElement* elPoi = new TiXmlElement("Point");

		// Vitesse
	Xml::SaveElement(elPoi, "Vitesse", m_Vitesse);

		// Position
	TiXmlElement* elPos = new TiXmlElement("Position");
	Xml::SaveAttribut(elPos, "x", m_Position.X);
	Xml::SaveAttribut(elPos, "y", m_Position.Y);
	Xml::SaveAttribut(elPos, "z", m_Position.Z);
	elPoi->LinkEndChild(elPos);
}

CNavette::CNavette( CMap *map )
	:CGeoObject( map )
{
	m_Point = -1;			// On a pas encore démarrer la navette
	m_distPoints = 0.0f;
	m_Vitesse = 0.0f;
	m_Deplacement = 0.0f;
}

CNavette::~CNavette(void)
{
}

void CNavette::Init()			// Initialisation de l'objet géométrique
{
	CGeoObject::Init();
}

void CNavette::Affiche()		// Fonction d'affichage de la navette
{
	glPushMatrix();

	glTranslatef( m_Position.X, m_Position.Y, m_Position.Z );

	CGeoObject::Affiche();						// Affichage

	glPopMatrix();
}

/*void CNavette::LitFichierNavette( CIfstreamMap &fichier )		// Lit un objet géo dans un fichier Map
{
	string mot;
	int nbrPoints=0;

	if( !( fichier>>mot ) )		// Nombre de points dans le trajet de la navette
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierNavette";
		throw CErreur( 1, erreur );
	}
	if( mot!="NbrPoints" )
	{
		string erreur( "\nFichier corrompu (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierNavette";
		throw CErreur( 2, erreur );
	}
	if( !( fichier>>nbrPoints ) )
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierNavette";
		throw CErreur( 3, erreur );
	}
	for( int i=0 ; i<nbrPoints ; i++ )
	{
		CPointNavette point;
		point.LitFichierPoint( fichier );
		m_ListePoints.push_back( point );
	}
}*/

/*bool CNavette::LitFichier( CIfstreamMap &fichier )
{
	LitFichierNavette( fichier );

	if( !CGeoObject::LitFichierGeoObject( fichier ) )
	{
		cerr << "\nErreur : Echec de lecture du fichier map associe a la navette (LitFichierGeoObject)";
		return false;
	}

	return true;	// La lecture s'est bien passée
}*/

/*bool CNavette::SaveNameType( ofstream &fichier )
{
	fichier << "\n\nNavette\n";	// Cette classe est une Navette
	return true;
}*/

/*bool CNavette::SaveFichierMap( ofstream &fichier )	// Sauve l'objet géo dans un fichier Map
{
	fichier << "\n\n\nNavette\n";						// Type objet navette

	fichier << "\n\tNbrPoints\t" <<	(unsigned int)m_ListePoints.size();			// Marge de la navette lors de son ouverture

	vector<CPointNavette>::iterator iter;
	for( iter=m_ListePoints.begin() ; iter!=m_ListePoints.end() ; iter++ )
		(*iter).SaveFichierPoint( fichier );

	if( !CGeoObject::SaveFichierMap( fichier ) )
		return false;		// La sauvegarde a echoue

	return true;	// La sauvegarde a abouti
}*/

bool CNavette::Save(TiXmlElement* element)	// Sauve l'objet géo dans un fichier Map
{
			// Sauve les données générales
	TiXmlElement* elGeo = new TiXmlElement("Geo");
	elGeo->SetAttribute("Ref", getReference());
	elGeo->SetAttribute("Nom", getName());
	elGeo->SetAttribute("Type", "Navette");
	element->LinkEndChild(elGeo);

	Xml::SaveElement(elGeo, "NbrPoints", (int)m_ListePoints.size());

	vector<CPointNavette>::iterator iter;
	for( iter=m_ListePoints.begin() ; iter!=m_ListePoints.end() ; iter++ )
		(*iter).SavePoint( elGeo );

	if( !CGeoObject::Save( elGeo ) )
		return false;		// La sauvegarde a echoue

	return true;	// La sauvegarde a abouti
}

int CNavette::prochainPoint( int i )
{
	CV3D ecart;
	i++;
	if( i<(int)m_ListePoints.size()-1 )
	{
			// Calcul du veteur vitesse da la navette entre les deux points
		ecart = m_ListePoints[ i+1 ].m_Position - m_ListePoints[ i ].m_Position;
		m_distPoints = ecart.norme();
		m_Deplacement = 0.0f;
		m_Position = m_ListePoints[ i ].m_Position;
		m_Position -= m_Centre;
		m_Vitesse = m_ListePoints[ i ].m_Vitesse;

		if( m_distPoints )
			m_Direction = ecart.directeur() * m_Vitesse;
		else
			m_Direction.setNull();
	}
	else if( i==m_ListePoints.size()-1 )
	{
			// Calcul du veteur vitesse da la navette entre les deux points
		ecart = m_ListePoints[ 0 ].m_Position - m_ListePoints[ i ].m_Position;
		m_distPoints = ecart.norme();
		m_Deplacement = 0.0f;
		m_Position = m_ListePoints[ i ].m_Position;
		m_Position -= m_Centre;
		m_Vitesse = m_ListePoints[ i ].m_Vitesse;

		if( m_distPoints )
			m_Direction = ecart.directeur() * m_Vitesse;
		else
			m_Direction.setNull();
	}
	else
	{
		i = 0;
			// Calcul du veteur vitesse da la navette entre les deux points
		ecart = m_ListePoints[ i+1 ].m_Position - m_ListePoints[ i ].m_Position;
		m_distPoints = ecart.norme();
		m_Deplacement = 0.0f;
		m_Position = m_ListePoints[ i ].m_Position;
		m_Position -= m_Centre;
		m_Vitesse = m_ListePoints[ i ].m_Vitesse;

		if( m_distPoints )
			m_Direction = ecart.directeur() * m_Vitesse;
		else
			m_Direction.setNull();
	}

	return i;
}

void CNavette::Refresh( CGame *game )
{
	if( m_ListePoints.size() > 1 )
	{
		if( m_Point == -1 )
			m_Point = prochainPoint( m_Point );

		m_Deplacement += m_Vitesse;
		if( (m_Deplacement >= m_distPoints) || (-m_Deplacement >= m_distPoints) )
			m_Point = prochainPoint( m_Point );

		m_Position += m_Direction;
	}
}

float CNavette::GereLaser( float pos[3], CV3D &Dir, float dist )
{
	float newPos[3];
	newPos[0] = pos[0] - m_Position.X;
	newPos[1] = pos[1] - m_Position.Y;
	newPos[2] = pos[2] + m_Position.Z;

	return CGeoObject::GereLaser( newPos, Dir, dist );
}

void CNavette::GereContactPlayer( const float pos[3], CPlayer *player )
{
	float newPos[3];
	newPos[0] = pos[0] - m_Position.X;
	newPos[1] = pos[1] - m_Position.Y;
	newPos[2] = pos[2] + m_Position.Z;

	CGeoObject::GereContactPlayer( newPos, player );
}

}	// JKT_PACKAGE_MOTEUR3D
