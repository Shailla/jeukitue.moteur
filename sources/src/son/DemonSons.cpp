

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

CDemonSons::CDemonSons()
{
LOGDEBUG(("CDemonSons::CDemonSons()%T", this));
}

CDemonSons::~CDemonSons()
{
LOGDEBUG(("CDemonSons::~CDemonSons()%T", this));
	set<CSon*>::iterator S;
	for( S=m_TabSon.begin() ; S!=m_TabSon.end() ; S++ )
		delete *S;
}

CSon3D* CDemonSons::CreateSon3D(const char *nomFichierSon)
{
LOGDEBUG(("CDemonSons::CreateSon3D(nomFichierSon=%s)%T", nomFichierSon, this));
	return (CSon3D*)CreateSon( nomFichierSon, SON_3D );
}

CSon* CDemonSons::CreateSon(const char *nomFichierSon, TYPE_SON type) // Charge un CSon � partir d'un fichier wave
{
LOGDEBUG(("CDemonSons::CreateSon(nomFichierSon=%s,type=%d)%T",nomFichierSon, type, this));
	CSon *son = 0;

	try
	{
		switch( type )
		{
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
	catch( int )		// R�ception d'une erreur de lecture du son
	{
		if( son )
			delete son;
		son = 0;	// La cr�ation du son n'a pas eu lieu
	}

	return son;
}

void CDemonSons::Play( CSon* id )	// Joue un son volatil donc sans retour d'identifiant de requ�te
{
	set<CSon*>::iterator p = m_TabSon.find( id );	// V�rifie que le son existe
	if( p==m_TabSon.end() ) {
		LOGWARN(("Erreur (CDemonSons) : Tentative de jouer un son qui n'existe pas."));
		return;
	}

	CReqSon *req = id->PlayID( false );		// Joue le son sans pause
	req->m_bVolatil = true;					// La requ�te sera supprim�e � sa fin
}

void CDemonSons::Delete( CReqSon *req ) {
LOGDEBUG(("CDemonSons::Delete(req=%x)%T", req, this));
	map<CReqSon*,CSon*>::iterator r = m_TabReq.find( req );	// Trouve la requ�te dans la liste du d�mon

	if( r!=m_TabReq.end() ) {
		// Destruction de la requ�te
		delete req;

		// Trouve et supprime la requ�te dans la liste du son associ�
		set<CReqSon*>::iterator rson = r->second->m_TabReq.find( req );
		if( rson!=r->second->m_TabReq.end() )
			r->second->m_TabReq.erase( rson );
		else
			LOGERROR((" Erreur (CDemonSons) : Delete impossible 1."));

		// Supprime la requ�te dans la liste du d�mon
		m_TabReq.erase( r );
	}
	else {
		LOGERROR(("Erreur (CDemonSons::Delete) : Delete impossible 2."));
	}
}

void CDemonSons::Delete( CSon *son ) {
LOGDEBUG(("CDemonSons::Delete(son=%x)%T", son, this));
	set<CSon*>::iterator s = m_TabSon.find( son );	// V�rifie que le son existe

	if( s==m_TabSon.end() ) {
		LOGERROR(("Tentative de detruire un son inexistant"));
		return;
	}

	m_TabSon.erase( s );	// Supprime le son de la liste du d�mon

	delete son;
}

void CDemonSons::Play( CReqSon *id )	// Relance la requ�te au d�but du son
{
	map<CReqSon*,CSon*>::iterator p = m_TabReq.find( id );	// V�rifie que la requ�te existe
	if( p!=m_TabReq.end() )
		id->Play();
	else
		LOGERROR(("Erreur : Requete son introuvable !!!"));
}

void CDemonSons::Erase( CReqSon *req ) {
LOGDEBUG(("CDemonSons::Erase(req=%x)%T", req, this));
	map<CReqSon*,CSon*>::iterator R = m_TabReq.find( req );

	if( R!=m_TabReq.end() )
		m_TabReq.erase( R );	// Supprime la requ�te de la liste redondante du d�mon
	else
		LOGERROR(("Erreur (CDemonSons::Delete) : Chose bizarre"));
}

CReqSon* CDemonSons::PlayID( CSon* id, bool pause )	// Joue un son avec retour d'identifiant de requ�te
{
	set<CSon*>::iterator p = m_TabSon.find( id );	// V�rifie que le son existe
	if( p==m_TabSon.end() ) {
		LOGERROR(("CSon introuvable !!!"));
		return 0;
	}

	CReqSon* req = id->PlayID( pause );
	req->m_bVolatil = false;		// Seul le demandeur de requ�te peut la supprimer

	return req;
}

void CDemonSons::Refresh() {	// V�rifie si des requ�tes volatiles sont arriv�es � leur fin
								// dans ce cas les supprime et oublie les
	LOGINFO(("REFRESH SONS, RESTE A CODER"));

	map<CReqSon*, CSon*>::iterator r,p;
	for( r=m_TabReq.begin() ; r!=m_TabReq.end() ; ) {	// Ne rien mettre � la fin du for !!!
		p = r++;

		if( p->first->m_bVolatil ) {
			if( FSOUND_IsPlaying( p->first->channel ) ) {	// Si la requ�te a fini son ex�cution
				FSOUND_StopSound( p->first->channel );	// Lib�re son canal
					// D�truit la requ�te
				delete p->first;
					// Trouve la et supprime la de la liste des requ�tes du son
				set<CReqSon*>::iterator rson = p->second->m_TabReq.find( p->first );
				p->second->m_TabReq.erase( rson );
					// Supprime la de la liste des requ�tes du d�mon
				m_TabReq.erase( p );
			}
		}
	}
}
