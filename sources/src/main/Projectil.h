
#ifndef __JKT__PROJECTIL_H
#define __JKT__PROJECTIL_H

class CPlayer;

class CProjectil {
protected:
	CPlayer *m_Player;

public:
	CProjectil( CPlayer *player );
	virtual ~CProjectil();

	virtual void Affiche() = 0;	// Affichage du projectil
	virtual bool Refresh(Uint32 now, float deltaTime) = 0;	// Effectue les calculs et les tests n�cessaires
};

#endif
