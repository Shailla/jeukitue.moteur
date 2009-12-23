
#ifndef __JKT__MOUVE_H
#define __JKT__MOUVE_H

class CGame;

namespace JKT_PACKAGE_MOTEUR3D
{

class CMouve
{
public:
	CMouve(void);
	~CMouve(void);

	virtual void Refresh( CGame *game )=0;
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

