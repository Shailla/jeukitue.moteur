/*
CDemonSons est le d�mon qui g�re les sons dans le jeu.

Il peut cr�er de nouveaux sons et les requ�tes associ�es.
Un son (CSon) contient les donn�es audio, par exemple le bruit d'une explosion de rocket
ou la musique du jeu. Chaque son peut �tre identifi� par son CSon*.
Un son peut �tre jou� plusieurs fois en m�me temps, cela se fait par l'interm�diaire des
requ�tes. Par exemple, on cr�e un son 'explosion', on cr�e alors durant la partie une
requ�te sur ce son chaque fois qu'une rocket explose. Il se peut bien-s�r que plusieurs rocket
explose en m�me temps.

Il existe 2 fa�ons de jouer un son :

	- la plus simple joue le son une seule fois sans que l'on puisse modifier la fa�on dont
	il est jou� en cours. Par exemple l'explosion d'une rocket est
	parfaitement localis�e dans l'espace. Dans ce cas la requ�te est cr�e de fa�on transparente
	par CDemonRequete et est d�truite une fois que le son a �t� jou�.

	- Avec la deuxi�me fa�on on obtient un identifiant de requ�te qui permet de modifier ses
	param�tres pendant qu'il est jou�. Par exemple le bruit d'une rocket doit se d�placer avec elle,
	une fois la requ�te cr�e on va donc l'informer r�guli�rement de sa nouvelle position spatiale
	gr�ce � son identifiant. La destruction d'une requ�te cr��e de cette fa�on doit �tre
	demad�e explicitement.
*/


#ifndef __JKT__DEMONSONS_H
#define __JKT__DEMONSONS_H

#include <set>
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
	std::map<CReqSon*,CSon*> m_TabReq;		// Table des identifiants des requ�tes existantes
private:
	void Erase( CReqSon *req );			// Enl�ve la requ�te de la liste des requ�tes
public:
	CDemonSons();
	~CDemonSons();

	CSon* CreateSon(const char *nomFichierSon, TYPE_SON type=SON_MONO);	// Charge un CSon � partir d'un fichier wave
	CSon3D* CreateSon3D(const char *nomFichierSon);	// Charge un CSon3D � partir d'un fichier wave
	void Delete( CSon *son );	// Supprime un son du d�mon (et ses requ�tes bien entendu)

	void Play( CSon* id );			// Joue un son sans retour d'identifiant de requ�te
	void Play( CReqSon *id );		// (re)joue un son qui poss�de d�j� une requ�te
	CReqSon* PlayID( CSon* id, bool pause=false );	// Joue un son avec retour d'identifiant de requ�te
	void Refresh();
	void Delete( CReqSon *req );	// Destruction/suppression d'une requ�te
};

}	// namespace jkt
#endif

