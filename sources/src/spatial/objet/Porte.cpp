
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <map>

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>

class CGame;

#include "spatial/XmlVocabulaire.h"
#include "util/Erreur.h"
#include "util/V3D.h"
#include "spatial/IfstreamMap.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/geo/GeoObject.h"
#include "son/DemonSons.h"
#include "main/Player.h"
#include "main/Game.h"
#include "main/Player.h"

#include "spatial/objet/Porte.h"

using namespace std;

namespace jkt
{

const char* CPorte::identifier = "Porte";

CPorte::CPorte( CMap *map ) : CGeoObject(map) {
	m_fMarge = 0.0f;
	m_fOuverture = 0.0f;
	m_fVitesse = 0.0f;
	m_fDeclenche = 0.0f;
}

CPorte::~CPorte(void) {
}

void CPorte::init() {			// Initialisation de l'objet g�om�trique
	CGeoObject::init();
	m_Position = m_Centre;
}

void CPorte::affiche() {		// Fonction d'affichage de la porte
	glPushMatrix();

	glTranslatef( 0.0f, m_fOuverture, 0.0f );	// Translation de la porte de la valeur de son ouverture
	CGeoObject::affiche();						// Affichage

	glPopMatrix();
}

bool CPorte::Save(TiXmlElement* element) {	// Sauve l'objet géo dans un fichier Map
	// Sauve les données générales
	TiXmlElement* elGeo = new TiXmlElement("Geo");
	elGeo->SetAttribute("Ref", getId());
	elGeo->SetAttribute("Nom", getName());
	elGeo->SetAttribute("Type", "Porte");
	element->LinkEndChild(elGeo);

	Xml::SaveElement(elGeo, "Marget", m_fMarge);	// Marge de la porte lors de son ouverture
	Xml::SaveElement(elGeo, "Vitesse", m_fVitesse);	// Vitesse de la porte
	Xml::SaveElement(elGeo, "Declenche", m_fDeclenche);	// Distance de la porte qui d�clenche son m�canisme

	if( !CGeoObject::Save( elGeo ) )
		return false;		// La sauvegarde a echoue

	return true;	// La sauvegarde a abouti
}

float CPorte::gereLaserPlayer(float pos[3], CV3D &Dir, float dist) {
	float newPos[3];

		// Ajustement de la position de la porte du � son ouverture (la porte monte <=> le joueur descend)
	newPos[ 0 ] = pos[ 0 ];
	newPos[ 1 ] = pos[ 1 ] - m_fOuverture;
	newPos[ 2 ] = pos[ 2 ];

	return CGeoObject::gereLaserPlayer( newPos, Dir, dist );
}

void CPorte::gereContactPlayer(float positionPlayer[3], CPlayer *player) {
	float newPos[3];

	// Ajustement de la position de la porte du � son ouverture (la porte monte <=> le joueur descend)
	newPos[ 0 ] = positionPlayer[ 0 ];
	newPos[ 1 ] = positionPlayer[ 1 ] - m_fOuverture;
	newPos[ 2 ] = positionPlayer[ 2 ];

	CGeoObject::gereContactPlayer(newPos, player);
}

void CPorte::refresh( CGame *game ) {
	bool bOuvre = false;	// La porte ne bouge pas par d�faut
	CPlayer *player;
	CV3D dist;
	int curseur = -1;
	float pos[3];

	while((player = game->nextPlayer(curseur))) {
		dist = m_Position;
		player->getPosition( pos );
		dist -  pos;

		// Un joueur est-il pr�t de la porte
		if( dist.norme() < m_fDeclenche  ) {
			bOuvre = true;	// Demande l'ouverture de la porte
			break;			// Sortie de la boucle car on sait que la porte doit s'ouvrir
		}
	}

	if( bOuvre ) {
		if( m_fOuverture < m_fMarge )		// Si la porte n'est pas enti�rement ouverte...
			m_fOuverture += m_fVitesse;		// ...commande son ouverture

		if( m_fOuverture > m_fMarge )		// Mais elle peut pas d�passer son ouverture maximale
			m_fOuverture = m_fMarge;
	}
	else {
		if( m_fOuverture > 0.0f )			// Si la porte n'est pas ferm�e...
			m_fOuverture -= m_fVitesse;		// commande sa fermeture

		if( m_fOuverture < 0.0f )			// Mais la porte peut pas descendre sous terre !
			m_fOuverture = 0.0f;
	}
}

}	// JktMoteur
