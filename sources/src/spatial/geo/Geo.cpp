
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "util/GenRef.h"
#include "spatial/IfstreamMap.h"

#include "spatial/geo/Geo.h"

namespace jkt
{
jkt::CGenRef CGeo::GENREF;

CGeo::CGeo( CMap *map ) {
	pMap = map;
	m_Reference = GENREF.genRef();	// G�n�re une r�f�rence pour l'objet
}

CGeo::CGeo(const CGeo& geo) {
	pMap = geo.pMap;
	_name = geo._name;
	m_Reference = GENREF.genRef();	// G�n�re une r�f�rence pour l'objet
}

CGeo::~CGeo() {
	GENREF.delRef(m_Reference);		// Lib�re la r�f�rence de l'objet
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
