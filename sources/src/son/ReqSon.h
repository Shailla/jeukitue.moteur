
#ifndef __JKT__REQSON_H
#define __JKT__REQSON_H

namespace jkt {

class CSon;

class CReqSon
{
public:
	CSon* m_IdSon;		// Identifiant du son associ� � la requ�te
	int channel;		// Canal utilis� par la requ�te
	bool m_bPause;
	bool m_bBoucle;
	bool m_bVolatil;	// Indique si la requ�te peut �tre effac�e une fois son ex�cution termin�e

public:
	CReqSon( CSon *id );
	~CReqSon();

	void Pause( bool indic );		// Met la requ�te en pause ou non
	void Boucle( bool indic );		// Joue le son associ� � la requ�te en boucle ou non
	void Play();					// Joue le son associ� en (re)partant du d�but
	bool IsPlaying();				// Indique si le son est en train d'�tre jou�
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

}	// namespace jkt
#endif
