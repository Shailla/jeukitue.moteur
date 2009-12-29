
#ifndef __JKT__REQSON_H
#define __JKT__REQSON_H

namespace JKT_PACKAGE_SON {

class CSon;

class CReqSon
{
public:
	CSon* m_IdSon;		// Identifiant du son associé à la requête
	int channel;		// Canal utilisé par la requête
	bool m_bPause;
	bool m_bBoucle;
	bool m_bVolatil;	// Indique si la requête peut être effacée une fois son exécution terminée

public:
	CReqSon( CSon *id );
	~CReqSon();

	void Pause( bool indic );		// Met la requête en pause ou non
	void Boucle( bool indic );		// Joue le son associé à la requête en boucle ou non
	void Play();					// Joue le son associé en (re)partant du début
	bool IsPlaying();				// Indique si le son est en train d'être joué
};

class CReqSonMono : public CReqSon
{
public:
	CReqSonMono(CSon *id);
	~CReqSonMono();
};

class CReqSonStereo : public CReqSon
{
public:
	CReqSonStereo(CSon *id);
	~CReqSonStereo();
};

class CReqSon3D : public CReqSon
{
	float m_Position[3];
	float m_Vitesse[3];
	bool m_bDoppler;

public:
	CReqSon3D(CSon *id);
	~CReqSon3D();

	void SetPosition( float pos[3] );
};

}	// namespace JKT_PACKAGE_SON
#endif
