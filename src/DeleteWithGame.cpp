
#include <set>

using namespace std;

#include "DeleteWithGame.h"

set<CDeleteWithGame*>* CDeleteWithGame::m_Tab = new set<CDeleteWithGame*>();

CDeleteWithGame::CDeleteWithGame()
{
	CDeleteWithGame::m_Tab->insert( this );
}

CDeleteWithGame::~CDeleteWithGame()
{
	set<CDeleteWithGame*>::iterator d = CDeleteWithGame::m_Tab->find( this );
	m_Tab->erase( d );
}

void CDeleteWithGame::DeleteAll()
{
	set<CDeleteWithGame*>::iterator d;
	set<CDeleteWithGame*>::iterator e;

	for( d=CDeleteWithGame::m_Tab->begin() ; d!=CDeleteWithGame::m_Tab->end() ; d++ )
		delete *d;
}
