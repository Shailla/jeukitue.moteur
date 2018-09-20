

#ifdef WIN32				// Pour FMOD
  #include <conio.h>		// Pour FMOD
  #include <windows.h>		// Pour FMOD
#else						// Pour FMOD
  #include "wincompat.h"	// Pour FMOD
#endif						// Pour FMOD

#include "util/Trace.h"
#include "fmod.h"

#include "Son.h"
#include "ReqSon.h"

#include "DemonSons.h"

using namespace std;
using namespace jkt;

CDemonSons::CDemonSons() {
LOGDEBUG(("CDemonSons::CDemonSons()%T", this));
}

CDemonSons::~CDemonSons() {
LOGDEBUG(("CDemonSons::~CDemonSons()%T", this));
	set<CSon*>::iterator S;

	for( S=m_TabSon.begin() ; S!=m_TabSon.end() ; S++ ) {
		delete *S;
	}
}

CSon3D* CDemonSons::createSon3D(const char *nomFichierSon) {
LOGDEBUG(("CDemonSons::CreateSon3D(nomFichierSon=%s)%T", nomFichierSon, this));
	return (CSon3D*)createSon(nomFichierSon, SON_3D);
}

CSon* CDemonSons::createSon(const char *nomFichierSon, TYPE_SON type) {	 // Charge un CSon � partir d'un fichier wave
LOGDEBUG(("CDemonSons::CreateSon(nomFichierSon=%s,type=%d)%T",nomFichierSon, type, this));
	CSon *son = 0;

	try {
		switch( type ) {
		case SON_MONO:
			son = new CSonMono( this, nomFichierSon );
			m_TabSon.insert( son );
			break;

		case SON_STEREO:
			son = new CSonStereo( this, nomFichierSon );
			m_TabSon.insert( son );
			break;

		case SON_3D:
			son = new CSon3D( this, nomFichierSon );
			m_TabSon.insert( son );
			break;

		default:
			LOGERROR((" Erreur : On cree un type de son qui n'existe pas !!!"));
			break;
		}
	}
	catch( int ) {// Réception d'une erreur de lecture du son
		if( son ) {
			delete son;
		}

		son = 0;	// La cr�ation du son n'a pas eu lieu
	}

	return son;
}

void CDemonSons::play(CSon* id) {		// Joue un son volatil donc sans retour d'identifiant de requ�te
	set<CSon*>::iterator p = m_TabSon.find( id );	// V�rifie que le son existe

	if( p==m_TabSon.end() ) {
		LOGWARN(("Erreur (CDemonSons) : Tentative de jouer un son qui n'existe pas."));
		return;
	}

	CReqSon *req = id->PlayID( false );		// Joue le son sans pause
	req->_volatil = true;					// La requ�te sera supprim�e � sa fin
}

void CDemonSons::remove( CReqSon *req ) {
LOGDEBUG(("CDemonSons::remove(req=%x)%T", req, this));
	map<CReqSon*,CSon*>::iterator r = m_TabReq.find( req );	// Trouve la requ�te dans la liste du d�mon

	if( r!=m_TabReq.end() ) {
		// Destruction de la requ�te
		delete req;

		// Trouve et supprime la requ�te dans la liste du son associ�
		set<CReqSon*>::iterator rson = r->second->m_TabReq.find( req );
		if( rson!=r->second->m_TabReq.end() )
			r->second->m_TabReq.erase( rson );
		else
			LOGERROR((" Erreur (CDemonSons) : remove impossible 1."));

		// Supprime la requ�te dans la liste du d�mon
		m_TabReq.erase( r );
	}
	else {
		LOGERROR(("Erreur (CDemonSons::remove) : remove impossible 2."));
	}
}

void CDemonSons::remove( CSon *son ) {
LOGDEBUG(("CDemonSons::remove(son=%x)%T", son, this));
	set<CSon*>::iterator s = m_TabSon.find( son );	// V�rifie que le son existe

	if( s==m_TabSon.end() ) {
		LOGERROR(("Tentative de detruire un son inexistant"));
		return;
	}

	m_TabSon.erase( s );	// Supprime le son de la liste du d�mon

	delete son;
}

void CDemonSons::play(CReqSon *id) {	// Relance la requête au début du son
	map<CReqSon*,CSon*>::iterator p = m_TabReq.find( id );	// Vérifie que la requ�te existe

	if(p!=m_TabReq.end()) {
		id->play();
	}
	else {
		LOGERROR(("Erreur : Requete son introuvable !!!"));
	}
}

void CDemonSons::Erase( CReqSon *req ) {
LOGDEBUG(("CDemonSons::Erase(req=%x)%T", req, this));
	map<CReqSon*,CSon*>::iterator R = m_TabReq.find( req );

	if( R!=m_TabReq.end() ) {
		m_TabReq.erase( R );	// Supprime la requ�te de la liste redondante du d�mon
	}
	else {
		LOGERROR(("Erreur (CDemonSons::Delete) : Chose bizarre"));
	}
}

CReqSon* CDemonSons::playID( CSon* id, bool pause ) {	// Joue un son avec retour d'identifiant de requ�te
	set<CSon*>::iterator p = m_TabSon.find( id );	// Vérifie que le son existe

	if( p==m_TabSon.end() ) {
		LOGERROR(("CSon introuvable !!!"));
		return 0;
	}

	CReqSon* req = id->PlayID( pause );
	req->_volatil = false;		// Seul le demandeur de requ�te peut la supprimer

	return req;
}

void CDemonSons::refresh() {	// Vérifie si des requêtes volatiles sont arrivées à leur fin
								// dans ce cas les supprime et oublie les
	LOGINFO(("REFRESH SONS, RESTE A CODER"));

	map<CReqSon*, CSon*>::iterator r,p;

	for( r=m_TabReq.begin() ; r!=m_TabReq.end() ; ) {	// Ne rien mettre à la fin du for !!!
		p = r++;

		if( p->first->_volatil ) {
			if( FSOUND_IsPlaying( p->first->_channel ) ) {	// Si la requête a fini son exécution
				FSOUND_StopSound( p->first->_channel );	// Libère son canal

				// Détruit la requête
				delete p->first;

				// Trouve la et supprime la de la liste des requêtes du son
				set<CReqSon*>::iterator rson = p->second->m_TabReq.find( p->first );
				p->second->m_TabReq.erase( rson );

				// Supprime la de la liste des requêtes du démon
				m_TabReq.erase( p );
			}
		}
	}
}
