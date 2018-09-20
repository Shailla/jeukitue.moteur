/*
CDemonSons est le démon qui gère les sons dans le jeu.

Il peut créer de nouveaux sons et les requêtes associées.
Un son (CSon) contient les données audio, par exemple le bruit d'une explosion de rocket
ou la musique du jeu. Chaque son peut être identifié par son CSon*.
Un son peut être joué plusieurs fois en même temps, cela se fait par l'intermédiaire des
requêtes. Par exemple, on crée un son 'explosion', on crée alors durant la partie une
requête sur ce son chaque fois qu'une rocket explose. Il se peut bien-sûr que plusieurs rocket
explose en même temps.

Il existe 2 façons de jouer un son :

	- la plus simple joue le son une seule fois sans que l'on puisse modifier la façon dont
	il est joué en cours. Par exemple l'explosion d'une rocket est
	parfaitement localisée dans l'espace. Dans ce cas la requête est crée de façon transparente
	par CDemonRequete et est détruite une fois que le son a été joué.

	- Avec la deuxième façon on obtient un identifiant de requête qui permet de modifier ses
	paramètres pendant qu'il est joué. Par exemple le bruit d'une rocket doit se déplacer avec elle,
	une fois la requête crée on va donc l'informer régulièrement de sa nouvelle position spatiale
	grâce à son identifiant. La destruction d'une requête créée de cette façon doit être
	demadée explicitement.
*/


#ifndef __JKT__DEMONSONS_H
#define __JKT__DEMONSONS_H

#include <set>
#include <string>
#include <map>

#include "fmod.h"

#include "son/TypeSon.h"

namespace jkt {

class CSon;
class CSon3D;
class CReqSon;
class CReqSonMono;
class CReqSonStereo;
class CReqSon3D;

class CDemonSons
{
friend class CSon;
public:
	std::set<CSon*> m_TabSon;				// Table des identifiants des sons existants
	std::map<CReqSon*,CSon*> m_TabReq;		// Table des identifiants des requêtes existantes
private:
	void Erase( CReqSon *req );				// Enlève la requête de la liste des requêtes
public:
	CDemonSons();
	~CDemonSons();

	CSon* createSon(const char *nomFichierSon, TYPE_SON type=SON_MONO);			// Charge un CSon à partir d'un fichier wave
	CSon3D* createSon3D(const char *nomFichierSon);								// Charge un CSon3D à partir d'un fichier wave
	void remove(CSon *son);														// Supprime un son du démon (et ses requêtes bien entendu)

	void play(CSon* id);														// Joue un son sans retour d'identifiant de requête
	void play(CReqSon *id);														// (re)joue un son qui possède déjà une requête
	CReqSon* playID(CSon* id, bool pause=false);								// Joue un son avec retour d'identifiant de requête
	void refresh();
	void remove(CReqSon *req);													// Destruction/suppression d'une requête
};

}	// namespace jkt
#endif

