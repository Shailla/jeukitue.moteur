
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
#include "Player.h"
#include "Game.h"

#include "Porte.h"

namespace JKT_PACKAGE_MOTEUR3D
{

const char* CPorte::identifier = "Porte";

CPorte::CPorte( CMap *map )
	:CGeoObject( map )
{
	m_fMarge = 0.0f;
	m_fOuverture = 0.0f;
	m_fVitesse = 0.0f;
	m_fDeclenche = 0.0f;
}

CPorte::~CPorte(void)
{
}

void CPorte::Init()			// Initialisation de l'objet géométrique
{
	CGeoObject::Init();
	m_Position = m_Centre;
}

void CPorte::Affiche()		// Fonction d'affichage de la porte
{
	glPushMatrix();

	glTranslatef( 0.0f, m_fOuverture, 0.0f );	// Translation de la porte de la valeur de son ouverture
	CGeoObject::Affiche();						// Affichage

	glPopMatrix();
}

/*void CPorte::LitFichierPorte( CIfstreamMap &fichier )		// Lit un objet géo dans un fichier Map
{
	string mot;

	if( !( fichier>>mot ) )		// Lecture de la distance parcourue par la porte lors de son ouverture
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierPorte";
		throw CErreur( 0, erreur );
	}
	if( mot!="Marge" )
	{
		string erreur( "\nFichier corrompu (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierPorte";
		throw CErreur( 0, erreur );
	}
	if( !( fichier>>m_fMarge ) )
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierPorte";
		throw CErreur( 0, erreur );
	}

	if( !( fichier>>mot ) )		// Lecture de la vitesse de la porte
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierPorte";
		throw CErreur( 0, erreur );
	}
	if( mot!="Vitesse" )
	{
		string erreur( "\nFichier corrompu (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierPorte";
		throw CErreur( 0, erreur );
	}
	if( !( fichier>>m_fVitesse ) )
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierPorte";
		throw CErreur( 0, erreur );
	}

	if( !( fichier>>mot ) )		// Distance de la porte qui déclenche son mécanisme
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierPorte";
		throw CErreur( 0, erreur );
	}
	if( mot!="Declenche" )
	{
		string erreur( "\nFichier corrompu (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierPorte";
		throw CErreur( 0, erreur );
	}
	if( !( fichier>>m_fDeclenche ) )
	{
		string erreur( "\nFin de fichier prematuree (" );
		erreur += fichier.getFileFullName();
		erreur += ") dans LitFichierPorte";
		throw CErreur( 0, erreur );
	}
}*/

/*bool CPorte::LitFichier( CIfstreamMap &fichier )
{
	LitFichierPorte( fichier );

	if( !CGeoObject::LitFichierGeoObject( fichier ) )
	{
		cerr << "\nErreur : Echec de lecture du fichier map associe a la porte (LitFichierGeoObject)";
		return false;
	}

	return true;	// La lecture s'est bien passée
}*/

/*bool CPorte::SaveNameType( ofstream &fichier )
{
	fichier << "\n\nPorte\n";	// Cette classe est une Porte
	return true;
}*/

/*bool CPorte::SaveFichierMap( ofstream &fichier )	// Sauve l'objet géo dans un fichier Map
{
	fichier << "\n\n\nPorte\n";						// Type objet porte

	fichier << "\n\tMarge\t" <<	m_fMarge;			// Marge de la porte lors de son ouverture
	fichier << "\n\tVitesse\t" << m_fVitesse;		// Vitesse de la porte
	fichier << "\n\tDeclenche\t" <<	m_fDeclenche;	// Distance de la porte qui déclenche son mécanisme

	if( !CGeoObject::SaveFichierMap( fichier ) )
		return false;		// La sauvegarde a echoue

	return true;	// La sauvegarde a abouti
}*/

bool CPorte::Save(TiXmlElement* element)	// Sauve l'objet géo dans un fichier Map
{
			// Sauve les données générales
	TiXmlElement* elGeo = new TiXmlElement("Geo");
	elGeo->SetAttribute("Ref", getReference());
	elGeo->SetAttribute("Nom", getName());
	elGeo->SetAttribute("Type", "Porte");
	element->LinkEndChild(elGeo);

	Xml::SaveElement(elGeo, "Marget", m_fMarge);	// Marge de la porte lors de son ouverture
	Xml::SaveElement(elGeo, "Vitesse", m_fVitesse);	// Vitesse de la porte
	Xml::SaveElement(elGeo, "Declenche", m_fDeclenche);	// Distance de la porte qui déclenche son mécanisme

	if( !CGeoObject::Save( elGeo ) )
		return false;		// La sauvegarde a echoue

	return true;	// La sauvegarde a abouti
}

float CPorte::GereLaser( float pos[3], CV3D &Dir, float dist )
{
	float newPos[3];

		// Ajustement de la position de la porte du à son ouverture (la porte monte <=> le joueur descend)
	newPos[ 0 ] = pos[ 0 ];
	newPos[ 1 ] = pos[ 1 ] - m_fOuverture;
	newPos[ 2 ] = pos[ 2 ];

	return CGeoObject::GereLaser( newPos, Dir, dist );
}

void CPorte::GereContactPlayer( const float pos[3], CPlayer *player )
{
	float newPos[3];

		// Ajustement de la position de la porte du à son ouverture (la porte monte <=> le joueur descend)
	newPos[ 0 ] = pos[ 0 ];
	newPos[ 1 ] = pos[ 1 ] - m_fOuverture;
	newPos[ 2 ] = pos[ 2 ];

	CGeoObject::GereContactPlayer( newPos, player );
}

void CPorte::Refresh( CGame *game )
{
	bool bOuvre = false;	// La porte ne bouge pas par défaut
	CPlayer *player;
	CV3D dist;
	int curseur = -1;
	float pos[3];

	while( game->pTabIndexPlayer->bSuivant( curseur ) )
	{
		player = game->pTabIndexPlayer->operator []( curseur );
		dist = m_Position;
		player->getPosition( pos );
		dist -  pos;
		if( dist.norme() < m_fDeclenche  )	// Un joueur est-il prêt de la porte
		{
			bOuvre = true;	// Demande l'ouverture de la porte
			break;			// Sortie de la boucle car on sait que la porte doit s'ouvrir
		}
	}

	if( bOuvre )
	{
		if( m_fOuverture < m_fMarge )	// Si la porte n'est pas entièrement ouverte
			m_fOuverture += m_fVitesse;		// Commande son ouverture

		if( m_fOuverture > m_fMarge )
			m_fOuverture = m_fMarge;	// Mais elle peut pas dépasser son ouverture maximale
	}
	else
	{
		if( m_fOuverture > 0.0f )	// Si la porte n'est pas fermée
			m_fOuverture -= m_fVitesse;		// Commande sa fermeture

		if( m_fOuverture < 0.0f )	// Mais la porte peut pas descendre sous terre !
			m_fOuverture = 0.0f;
	}
}

}	// JKT_PACKAGE_MOTEUR3D
