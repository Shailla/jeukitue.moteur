
#include <set>

using namespace std;

#include "DeleteWithGame.h"

CDeleteWithGame::CDeleteWithGame()
{
	m_Tab.insert( this );
}

CDeleteWithGame::~CDeleteWithGame()
{
	set<CDeleteWithGame*>::iterator d = m_Tab.find( this );
	m_Tab.erase( d );
}

void CDeleteWithGame::DeleteAll()
{
	set<CDeleteWithGame*>::iterator d;
	set<CDeleteWithGame*>::iterator e;

	for( d=m_Tab.begin() ; d!=m_Tab.end() ; d++ )
		delete *d;
}
