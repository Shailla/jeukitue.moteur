
#ifndef __JKT__REQSON_H
#define __JKT__REQSON_H

namespace jkt {

class CSon;

class CReqSon {
public:
	CSon* _son;		// Identifiant de la requête son
	int _channel;		// Canal de la requête
	bool _pause;
	bool _boucle;
	bool _volatil;	// Indique si la requête peut être effacée une fois son exécution terminée

public:
	CReqSon(CSon* id );
	~CReqSon();

	void pause(bool pause);			// Met la requête en pause ou non
	void boucle(bool boucle);		// Joue le son associé à la requête en boucle ou non
	void play();					// Joue le son associé en (re)partant du début
	bool isPlaying();				// Indique si le son est en train d'être joué
};

class CReqSonMono : public CReqSon {
public:
	CReqSonMono(CSon* id);
	~CReqSonMono();
};

class CReqSonStereo : public CReqSon {
public:
	CReqSonStereo(CSon* id);
	~CReqSonStereo();
};

class CReqSon3D : public CReqSon {
	float _position[3];
	float _vitesse[3];
	bool _doppler;

public:
	CReqSon3D(CSon* id);
	~CReqSon3D();

	void setPosition(const float pos[3]);
};

}	// namespace jkt
#endif
