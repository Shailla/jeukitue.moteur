

#ifdef WIN32				// Pour FMOD
  #include <conio.h>		// Pour FMOD
  #include <windows.h>		// Pour FMOD
#else						// Pour FMOD
  #include "wincompat.h"	// Pour FMOD
#endif						// Pour FMOD
#include <iostream>

#include <map>

#include "fmod.h"
#include "fmod_errors.h"
#include "util/Trace.h"

#include "Son.h"

#include "ReqSon.h"
#include "DemonSons.h"

using namespace std;
using namespace jkt;

CSon::CSon( CDemonSons* p,const char *name )
{
LOGDEBUG(("CSon::CSon(nom=%s,...)%T", name, this ));
	nom = name;
	m_Sample = 0;
	pDemon = p;
}

CSon::~CSon()
{
LOGDEBUG(("CSon::~CSon() nom=%s%T", nom.c_str(), this ));

	set<CReqSon*>::iterator p;		// Destruction de toutes les requêtes sur ce son
	for( p = m_TabReq.begin() ; p != m_TabReq.end() ; p++ )
	{
		pDemon->Erase( *p );		// Supprime la requête de la liste appartenant au démon
		delete *p;
	}
	m_TabReq.clear();	// Vide la liste des requêtes du son (pas indispensable vu qu'elle va être détruite)

	if( !m_Sample )		// Destruction des échantillions du son
		FSOUND_Sample_Free( m_Sample );
}

CSonMono::CSonMono(CDemonSons* p, const char *nomFichierSon)	// Constructeur avec lecture fichier son
	:CSon( p, nomFichierSon )
{
LOGDEBUG(("CSonMono::CSonMono(nomFichierSon=%s)%T", nomFichierSon, this ));
	m_Sample = FSOUND_Sample_Load( FSOUND_FREE, nomFichierSon, FSOUND_MONO, 0, 0 );

	if( m_Sample==0 )
	{
LOGDEBUG(("CSonMono::CSonMono() %s%T", nomFichierSon, this ));
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur FMOD (" << nomFichierSon << ") :" << FMOD_ErrorString(FSOUND_GetError());
		throw (int)1;	// Envoie d'une erreur de lecture de fichier
	}
}

CSonMono::~CSonMono()
{
LOGDEBUG(("CSonMono::~CSonMono() nom=%sT", nom.c_str(), this ));
}

CSonStereo::CSonStereo(CDemonSons* p,const char *nomFichierSon)	// Constructeur avec lecture fichier son
	:CSon( p, nomFichierSon )
{
LOGDEBUG(("CSonStereo::CSonStereo(nomFichierSon=%s,...)%T", nomFichierSon, this ));
	m_Sample = FSOUND_Sample_Load( FSOUND_FREE, nomFichierSon, FSOUND_STEREO, 0, 0 );

	if( m_Sample==0 )
	{
LOGDEBUG(("CSonStereo::CSonStereo() %s%T", nomFichierSon, this ));
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur FMOD (" << nomFichierSon << ") :" << FMOD_ErrorString(FSOUND_GetError());
		throw (int)1;	// Envoie d'une erreur de lecture de fichier
	}
}

CSonStereo::~CSonStereo()
{
LOGDEBUG(("CSonStereo::~CSonStereo() nom=%s%T", nom.c_str(), this ));
}

CSon3D::CSon3D( CDemonSons* p,const char *nomFichierSon )	// Constructeur avec lecture fichier son
	:CSon( p, nomFichierSon )
{
LOGDEBUG(("CSon3D::CSon3D(nomFichierSon=%s,...)%T", nomFichierSon, this ));
	m_Sample = FSOUND_Sample_Load( FSOUND_FREE, nomFichierSon, FSOUND_HW3D, 0, 0 );

	if( m_Sample==0 ) {
LOGDEBUG(("CSon3D::CSon3D() %s%T", nomFichierSon, this ));
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur FMOD (" << nomFichierSon << ") :" << FMOD_ErrorString(FSOUND_GetError());
		throw (int)1;	// Envoie d'une erreur de lecture de fichier
	}
}

CSon3D::~CSon3D() {
LOGDEBUG(("CSon3D::~CSon3D() nom=%s%T", nom.c_str(), this ));
}

CReqSon *CSonMono::PlayID( bool pause ) {
	CReqSonMono *req = new CReqSonMono( this );
	m_TabReq.insert( req );
	pDemon->m_TabReq.insert( pair<CReqSon*,CSon*>( req, this ) );
	req->m_bPause = pause;

	if( !pause )
		req->channel = FSOUND_PlaySound( FSOUND_FREE, m_Sample );

	return req;
}

CReqSon *CSonStereo::PlayID( bool pause ) {
	CReqSonStereo *req = new CReqSonStereo( this );
	m_TabReq.insert( req );
	pDemon->m_TabReq.insert( pair<CReqSon*,CSon*>( req, this ) );
	req->m_bPause = pause;

	if( !pause )
		req->channel = FSOUND_PlaySound( FSOUND_FREE, m_Sample );

	return req;
}

CReqSon* CSon3D::PlayID( bool pause )
{
	CReqSon3D *req = new CReqSon3D( this );
	m_TabReq.insert( req );
	pDemon->m_TabReq.insert( pair<CReqSon*,CSon*>( req, this ) );
	req->m_bPause = pause;

	if( !pause )
		req->channel = FSOUND_PlaySound( FSOUND_FREE, m_Sample );

	return req;
}
