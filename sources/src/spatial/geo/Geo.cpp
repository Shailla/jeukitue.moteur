
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "util/GenRef.h"
#include "spatial/IfstreamMap.h"

#include "spatial/geo/Geo.h"

namespace JktMoteur
{
JktUtils::CGenRef CGeo::GENREF;

CGeo::CGeo( CMap *map )
{
	pMap = map;
	m_Reference = GENREF.genRef();	// Génère une référence pour l'objet
}

CGeo::~CGeo()
{
	GENREF.delRef(m_Reference);		// Libère la référence de l'objet
}

CMap *CGeo::getMap()
{	return pMap;	}

void CGeo::setMap(CMap* map) {
	pMap = map;
}

unsigned int CGeo::getReference()
{
	return m_Reference;
}

void CGeo::initGL() {}

void CGeo::freeGL() {}

void CGeo::setName( const string &nom ) {
	_name.clear();
	_name = nom;
}

const char *CGeo::getName() {
	return _name.c_str();
}

bool CGeo::Contact( const float pos[3], float dist ) {
	return false;
}

}	// JktMoteur
