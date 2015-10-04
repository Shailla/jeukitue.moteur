

#include <iostream>
#ifdef WIN32				// Pour FMOD
  #include <conio.h>		// Pour FMOD
  #include <windows.h>		// Pour FMOD
#else						// Pour FMOD
  #include "wincompat.h"	// Pour FMOD
#endif						// Pour FMOD

using namespace std;

#include "util/Trace.h"
#include "fmod.h"

#include "Son.h"
#include "ReqSon.h"

#include "DemonSons.h"

using namespace JktSon;

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

CSon* CDemonSons::CreateSon(const char *nomFichierSon, TYPE_SON type) // Charge un CSon à partir d'un fichier wave
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
			cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : On cree un type de son qui n'existe pas !!!";
			break;
		}
	}
	catch( int )		// Réception d'une erreur de lecture du son
	{
		if( son )
			delete son;
		son = 0;	// La création du son n'a pas eu lieu
	}

	CSon* id = son;
	return id;
}

void CDemonSons::Play( CSon* id )	// Joue un son volatil donc sans retour d'identifiant de requête
{
	set<CSon*>::iterator p = m_TabSon.find( id );	// Vérifie que le son existe
	if( p==m_TabSon.end() )
	{
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur (CDemonSons) : Tentative de jouer un son qui n'existe pas.";
		return;
	}

	CReqSon *req = id->PlayID( false );		// Joue le son sans pause
	req->m_bVolatil = true;		// La requête sera supprimée à sa fin
}

void CDemonSons::Delete( CReqSon *req )
{
LOGDEBUG(("CDemonSons::Delete(req=%x)%T", req, this));
	map<CReqSon*,CSon*>::iterator r = m_TabReq.find( req );	// Trouve la requête dans la liste du démon

	if( r!=m_TabReq.end() ) {
			// Destruction de la requête
		delete req;
			// Trouve et supprime la requête dans la liste du son associé
		set<CReqSon*>::iterator rson = r->second->m_TabReq.find( req );
		if( rson!=r->second->m_TabReq.end() )
			r->second->m_TabReq.erase( rson );
		else
			cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur (CDemonSons) : Delete impossible 1.";
			// Supprime la requête dans la liste du démon
		m_TabReq.erase( r );
	}
	else {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur (CDemonSons::Delete) : Delete impossible 2.";
	}
}

void CDemonSons::Delete( CSon *son )
{
LOGDEBUG(("CDemonSons::Delete(son=%x)%T", son, this));
	set<CSon*>::iterator s = m_TabSon.find( son );	// Vérifie que le son existe
	if( s==m_TabSon.end() )
	{
		cerr << endl << __FILE__ << ":" << __LINE__ << " Tentative de detruire un son inexistant";
		return;
	}

	m_TabSon.erase( s );	// Supprime le son de la liste du démon

	delete son;
}

void CDemonSons::Play( CReqSon *id )	// Relance la requête au début du son
{
	map<CReqSon*,CSon*>::iterator p = m_TabReq.find( id );	// Vérifie que la requête existe
	if( p!=m_TabReq.end() )
		id->Play();
	else
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Requete son introuvable !!!";
}

void CDemonSons::Erase( CReqSon *req )
{
LOGDEBUG(("CDemonSons::Erase(req=%x)%T", req, this));
	map<CReqSon*,CSon*>::iterator R = m_TabReq.find( req );
	if( R!=m_TabReq.end() )
		m_TabReq.erase( R );	// Supprime la requête de la liste redondante du démon
	else
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur (CDemonSons::Delete) : Chose bizarre";
}

CReqSon* CDemonSons::PlayID( CSon* id, bool pause )	// Joue un son avec retour d'identifiant de requête
{
	set<CSon*>::iterator p = m_TabSon.find( id );	// Vérifie que le son existe
	if( p==m_TabSon.end() )
	{
		cerr << endl << __FILE__ << ":" << __LINE__ << " ERREUR : CSon introuvable !!!";
		return 0;
	}

	CReqSon* req = id->PlayID( pause );
	req->m_bVolatil = false;		// Seul le demandeur de requête peut la supprimer

	return req;
}

void CDemonSons::Refresh()	// Vérifie si des requêtes volatiles sont arrivées à leur fin
{							// dans ce cas les supprime et oublie les
	cout << endl << "\nREFRESH SONS, RESTE A CODER";

	map<CReqSon*, CSon*>::iterator r,p;
	for( r=m_TabReq.begin() ; r!=m_TabReq.end() ; )	// Ne rien mettre à la fin du for !!!
	{
		p = r++;
		if( p->first->m_bVolatil )
		{
			if( FSOUND_IsPlaying( p->first->channel ) )	// Si la requête a fini son exécution
			{
				FSOUND_StopSound( p->first->channel );	// Libère son canal
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
