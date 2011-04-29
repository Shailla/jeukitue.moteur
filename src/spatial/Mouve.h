
#ifndef __JKT__MOUVE_H
#define __JKT__MOUVE_H

class CGame;

namespace JktMoteur
{

class CMouve
{
public:
	CMouve(void);
	virtual ~CMouve(void);

	virtual void Refresh( CGame *game )=0;
};

}	// JktMoteur

#endif

