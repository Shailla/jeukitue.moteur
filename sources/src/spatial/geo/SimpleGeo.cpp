
#include <string>
#include <fstream>
#include <set>
#include <cmath>
#include <map>
#include <sstream>

#ifdef WIN32
#include <windows.h>
#endif
#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glext.h>

class CGame;

extern int JKT_RenderMode;

#include "spatial/XmlVocabulaire.h"
#include "util/Tableau.cpp"
#include "util/StringUtils.h"
#include "util/math_vectoriel.h"
#include "util/V3D.h"
#include "spatial/IfstreamMap.h"
#include <spatial/basic/Geometrical.h>
#include <spatial/basic/Refreshable.h>
#include "spatial/light/Light.h"
#include "spatial/Map.h"
#include "son/DemonSons.h"
#include "spatial/contact.h"
#include "spatial/geo/GeoMaker.h"
#include "main/Player.h"

#include "SimpleGeo.h"

using namespace std;
using namespace jkt;

namespace jkt
{
class CGeoObject;
class CPorte;
class CNavette;

const char* CSimpleGeo::identifier = "SimpleGeo";

//CONSTRUCTEURS
CSimpleGeo::CSimpleGeo(CMap* map, const string& name, unsigned int nbrVertex, float* vertex, unsigned int nbrFaces,
		int* faces, float* color, bool solid) : MapObject(map, MapObject::GEO) {
	setName(name);	// Nom de l'objet

	_color[0] = color[0];			// Couleur de l'objet
	_color[1] = color[1];
	_color[2] = color[2];

	m_TabVertex = 0;				// Pointeur sur le tableau de sommets
	m_TabFaces = 0;				// Pointeur sur le tableau d'index des sommets

	setVertex(nbrVertex, vertex);	// Sommets
	setFaces(nbrFaces, faces);		// Indexation des sommets

	_bSolid = solid;				// Objet solide ou �th�reux

	_pNormalTriangle = 0;			// Sera initialis� par Init()

	_minX = _maxX = _minY = _maxY = _minZ = _maxZ = 0.0f;
	_rayon = 0.0f;
}

CSimpleGeo::CSimpleGeo(CMap* map) : MapObject(map, MapObject::GEO) {
	_color[0] = 1.0f;	// Couleur de l'objet
	_color[1] = 1.0f;
	_color[2] = 1.0f;

	m_TabVertex = 0;			// Pointeur sur le tableau de sommets
	m_TabFaces = 0;			// Pointeur sur le tableau d'index des sommets

	_numFaces = 0;
	_numVertex = 0;

	_bSolid = true;			// Objet solide par d�faut

	_pNormalTriangle = 0;		// Sera initialis� par Init()

	_minX = _maxX = _minY = _maxY = _minZ = _maxZ = 0.0f;
	_rayon = 0.0f;
}

CSimpleGeo::CSimpleGeo(const CSimpleGeo& other) : MapObject(other) {
	tostring = other.tostring;

	m_TabVertex = 0;			// Pointeur sur le tableau de sommets
	m_TabFaces = 0;			// Pointeur sur le tableau d'index des sommets

	_numFaces = 0;
	_numVertex = 0;

	// Bulle
	_minX = other._minX;
	_minY = other._minY;
	_minZ = other._minZ;
	_maxX = other._maxX;
	_maxY = other._maxY;
	_maxZ = other._maxZ;

	// Centre
	_centre[0] = other._centre[0];
	_centre[1] = other._centre[1];
	_centre[2] = other._centre[2];

	// Rayon
	_rayon = other._rayon;

	// Couleur
	_color[0] = other._color[0];
	_color[1] = other._color[1];
	_color[2] = other._color[2];

	// Solidité
	_bSolid = other._bSolid;

	// Nombres
	_numVertex = other._numVertex;
	_numFaces = other._numFaces;

	// Sommets
	setVertex(other._numVertex, other.m_TabVertex);

	// Faces
	setFaces(other._numFaces, other.m_TabFaces);

	// Normales
	_pNormalTriangle = 0;

	if(other._pNormalTriangle) {
		_pNormalTriangle = new float[ 3*_numFaces ];

		for(int i=0 ; i<3*_numFaces ; i++) {
			_pNormalTriangle[i] = other._pNormalTriangle[i];
		}
	}
}

CSimpleGeo::~CSimpleGeo() {
	if( m_TabVertex ) {
		delete[] m_TabVertex;
		m_TabVertex = 0;
	}

	if( m_TabFaces ) {
		delete[] m_TabFaces;
		m_TabFaces = 0;
	}

	if( _pNormalTriangle ) {
		delete[] _pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs pr�liminaires � la gestion des contacts)
		_pNormalTriangle = 0;
	}
}

MapObject* CSimpleGeo::clone() {
	return new CSimpleGeo(*this);
}

void CSimpleGeo::setVertex(int num, float *tab) {
	if( m_TabVertex )	// Destruction de l'ancien tableau de sommets s'il existe
		delete[] m_TabVertex;

	_numVertex = num;	// Nombre de sommets

	m_TabVertex = new float[ 3*_numVertex ];	// Cr�ation du tableau de sommets

	for( int i=0 ; i< num ; i++ )	// Copie des donn�es du tableau
		for( int j=0 ; j<3 ; j++ )
			m_TabVertex[ (i*3)+j ] = tab[ (i*3)+j ];
}

void CSimpleGeo::init() throw(CErreur) {
	MinMax();			// Mesure les minimums et maximums de l'objet g�o
	Bulle();			// Mesure le centre et le rayon de la sph�re englobant l'objet g�o
	ConstruitBase();	// Construit la table des vecteurs normaux
}

void CSimpleGeo::setFaces(int num, int *tab) {
	if( m_TabFaces )	// Destruction de l'ancien tableau d'indices de sommets s'il existe
		delete[] m_TabFaces;

	_numFaces = num;	// Nombre d'indices de sommets

	m_TabFaces = new int[3*_numFaces];	// Cr�ation du tableau d'indices de sommets

	for( int i=0 ; i< _numFaces ; i++ ) {		// Copie des donn�es du tableau
		for( int j=0 ; j<3 ; j++ ) {	// Copie des faces
			m_TabFaces[ (i*3)+j ] = tab[ (i*3)+j ];
		}
	}
}

void CSimpleGeo::initGL() {
	glGenBuffers(VBO_BUFFER_SIZE, m_VboBufferNames);

	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, _numVertex*3*sizeof(float), m_TabVertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboBufferNames[VBO_FACES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numFaces*3*sizeof(unsigned int), m_TabFaces, GL_STATIC_DRAW);
}

void CSimpleGeo::freeGL() {
	glDeleteBuffers(VBO_BUFFER_SIZE, m_VboBufferNames);
}

void CSimpleGeo::affiche() {
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glColor3fv(_color);

	glLineWidth(1);

	// Attachement des VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_VERTEX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboBufferNames[VBO_FACES]);

	// Affichage
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(JKT_RenderMode, 3*_numFaces, GL_UNSIGNED_INT, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}

const char* CSimpleGeo::toString() {
	ostringstream ttt;
	ttt << identifier << " Nom=" << getName() << " Coul={" << _color[0];
	ttt << "," << _color[1] << "," << _color[2] << "}";

	tostring = ttt.str();

	return tostring.c_str();
}

void CSimpleGeo::afficheHighlighted(float r, float v, float b) {
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glColor3f(r, v, b);

	glLineWidth(1);

	// Attachement des VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_VERTEX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboBufferNames[VBO_FACES]);

	// Affichage
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(JKT_RenderMode, 3*_numFaces, GL_UNSIGNED_INT, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void CSimpleGeo::MinMax() {
	_minX = _maxX = m_TabVertex[0];
	_minY = _maxY = m_TabVertex[1];
	_minZ = _maxZ = m_TabVertex[2];

	for(int i=1 ; i<_numVertex ; i++) {
		if( m_TabVertex[3*i] < _minX )		//r�cup�ration des coordonn�es du pav� englobant
			_minX = m_TabVertex[3*i];		//l'objet g�o
		if( m_TabVertex[(3*i)+1] < _minY )
			_minY = m_TabVertex[(3*i)+1];
		if( m_TabVertex[(3*i)+2] < _minZ )
			_minZ = m_TabVertex[(3*i)+2];

		if( m_TabVertex[3*i] > _maxX )
			_maxX = m_TabVertex[3*i];
		if( m_TabVertex[(3*i)+1] > _maxY )
			_maxY = m_TabVertex[(3*i)+1];
		if( m_TabVertex[(3*i)+2] > _maxZ )
			_maxZ = m_TabVertex[(3*i)+2];
	}
}

void CSimpleGeo::Bulle() {
	float r0, r1, r2;

	// Calcul du centre de la sph�re � partir des valeurs min/max
	_centre[0] = (_minX + _maxX)/2.0f;
	_centre[1] = (_minY + _maxY)/2.0f;
	_centre[2] = (_minZ + _maxZ)/2.0f;

	// Recherche du rayon de la sph�re
	r0 = fabsf(_minX - _maxX);
	r1 = fabsf(_minY - _maxY);
	r2 = fabsf(_minZ - _maxZ);

	_rayon = sqrtf( (r0*r0) + (r1*r1) + (r2*r2) );
}

void CSimpleGeo::ConstruitBase()
{
	float X[3], Y[3];

	if( _pNormalTriangle )
		delete[] _pNormalTriangle;

	_pNormalTriangle = new float[ 3*_numFaces ];

	for( int i=0; i<_numFaces; i++ ) {		//pour chaque triangle de l'objet g�o.
		// X = vecteur cot� AB
		X[0] = m_TabVertex[ 3*m_TabFaces[(3*i)+1]] - m_TabVertex[ 3*m_TabFaces[3*i] ];
		X[1] = m_TabVertex[ (3*m_TabFaces[(3*i)+1])+1 ] - m_TabVertex[ (3*m_TabFaces[3*i])+1 ];
		X[2] = m_TabVertex[ (3*m_TabFaces[(3*i)+1])+2 ] - m_TabVertex[ (3*m_TabFaces[3*i])+2 ];

		// Y = vecteur cot� AC
		Y[0] = m_TabVertex[3*m_TabFaces[(3*i)+2]] - m_TabVertex[3*m_TabFaces[3*i]];
		Y[1] = m_TabVertex[(3*m_TabFaces[(3*i)+2])+1] - m_TabVertex[(3*m_TabFaces[3*i])+1];
		Y[2] = m_TabVertex[(3*m_TabFaces[(3*i)+2])+2] - m_TabVertex[(3*m_TabFaces[3*i])+2];

		produitVectoriel(X, Y, &_pNormalTriangle[3*i]);	// calcul du vecteur normal au plan
		// du triangle
		normalise( &_pNormalTriangle[3*i] );		// normalise ce vecteur
	}
}

void CSimpleGeo::echangeXY() {	// Echange les axes X et Y de l'objet
	float varX, varY;

	for( int i=0 ; i<_numVertex ; i++ ) {
		varX = m_TabVertex[ 3*i ];
		varY = m_TabVertex[ (3*i)+1 ];

		m_TabVertex[ 3*i ] = varY;
		m_TabVertex[ (3*i)+1 ] = varX;
	}
}

void CSimpleGeo::echangeXZ() {	// Echange les axes X et Y de l'objet
	float varX, varZ;

	for( int i=0 ; i<_numVertex ; i++ ) {
		varX = m_TabVertex[ 3*i ];
		varZ = m_TabVertex[ (3*i)+2 ];

		m_TabVertex[ 3*i ] = varZ;
		m_TabVertex[ (3*i)+2 ] =varX;
	}
}

void CSimpleGeo::echangeYZ() {	// Echange les axes X et Y de l'objet
	float varY, varZ;

	for( int i=0 ; i<_numVertex ; i++ ) {
		varY = m_TabVertex[ (3*i)+1 ];
		varZ = m_TabVertex[ (3*i)+2 ];

		m_TabVertex[ (3*i)+1 ] = varZ;
		m_TabVertex[ (3*i)+2 ] =varY;
	}
}

void CSimpleGeo::scale( float scaleX, float scaleY, float scaleZ ) {
	for( int i=0 ; i<_numVertex ; i++ ) {
		m_TabVertex[ (3*i) ] *= scaleX;
		m_TabVertex[ (3*i)+1 ] *= scaleY;
		m_TabVertex[ (3*i)+2 ] *= scaleZ;
	}
}

void CSimpleGeo::translate( float x, float y, float z ) {
	for( int i=0 ; i<_numVertex ; i++ ) {
		m_TabVertex[ (3*i)+0 ] += x;
		m_TabVertex[ (3*i)+1 ] += y;
		m_TabVertex[ (3*i)+2 ] += z;
	}
}

void CSimpleGeo::Color( float r, float g, float b ) {
	_color[0] = r;
	_color[1] = g;
	_color[2] = b;
}

bool CSimpleGeo::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) throw(CErreur) {
	// Type
	const char* geoType = el->Value();

	if(strcmp(geoType, Xml::GEOSIMPLE)) {
		throw CErreur("Fichier Map corrompu CSimpleGeo 3");
	}

	// Nom
	const char* nom = el->Attribute(Xml::NOM);

	if(nom) {
		setName(nom);
	}

	// Abstract
	const char* attr = el->Attribute(Xml::ABSTRACT);
	bool abstract = Xml::LitBooleanNotMandatory(attr, false);
	setAbstract(abstract);

	// Référence
	const char* reference = el->Attribute(Xml::REF);

	if(reference) {
		_reference = reference;
	}

	// Solidité
	_bSolid = Xml::LitSolidite(el);

	// Couleur
	Xml::LitCouleur3fv(el, Xml::COULEUR, _color);

	// Sommets
	m_TabVertex = CGeoMaker::LitVertex(el, _numVertex);

	// Index des sommets
	m_TabFaces = CGeoMaker::LitFaces(el, _numFaces);

	return true;
}

bool CSimpleGeo::Save(TiXmlElement* element) throw(CErreur) {
	// Sauve les donn�es g�n�rales
	TiXmlElement* elGeo = new TiXmlElement(Xml::GEOSIMPLE);
	elGeo->SetAttribute(Xml::REF, getId());
	elGeo->SetAttribute(Xml::NOM, getName());
	if(isAbstract()) {
		elGeo->SetAttribute(Xml::ABSTRACT, "true");
	}
	element->LinkEndChild(elGeo);

	// Solidit�
	CGeoMaker::SaveSolidite(elGeo, _bSolid);

	// Couleur
	CGeoMaker::SaveCouleur3fv(elGeo, Xml::COULEUR, _color);

	// Sommets
	CGeoMaker::SaveVertex(elGeo, _numVertex, m_TabVertex);

	// Index des sommets
	CGeoMaker::SaveFaces(elGeo, _numFaces, m_TabFaces);

	return true;
}

float CSimpleGeo::testContactTriangle(unsigned int i, const float posPlayer[3], float dist) {

	// Teste s'il y a contact entre un point P et le i° triangle de l'objet géo
	int *indices = &m_TabFaces[3*i];
	float *normal = &_pNormalTriangle[3*i];
	float A[3], B[3], C[3], F[3], G[3], X[3], Y[3], Z[3];
	float distanceW;

	// Z = vecteur AP, P personnage
	Z[0] =  posPlayer[0] - m_TabVertex[ (3*indices[0])+0 ];
	Z[1] =  posPlayer[1] - m_TabVertex[ (3*indices[0])+1 ];
	Z[2] = -posPlayer[2] - m_TabVertex[ (3*indices[0])+2 ];

	//distance entre le plan du triangle et le joueur
	distanceW = produitScalaire(normal, Z);

	if( distanceW >= dist )
		return 1000.0f;		// Y'a pas contact

	if( distanceW <= - dist )
		return 1000.0f;		// Y'a pas contact

	X[0] = m_TabVertex[ (3*indices[1])+0 ] - m_TabVertex[ (3*indices[0])+0 ];
	X[1] = m_TabVertex[ (3*indices[1])+1 ] - m_TabVertex[ (3*indices[0])+1 ];
	X[2] = m_TabVertex[ (3*indices[1])+2 ] - m_TabVertex[ (3*indices[0])+2 ];

	produitVectoriel( X, normal, A );
	normalise( A );
	// test position P / droite AB
	if( produitScalaire(A, Z) >= dist )	// avant c'était 0.0
		return 1000.0f;

	Y[0] = m_TabVertex[ (3*indices[2])+0 ] - m_TabVertex[ (3*indices[0])+0 ];
	Y[1] = m_TabVertex[ (3*indices[2])+1 ] - m_TabVertex[ (3*indices[0])+1 ];
	Y[2] = m_TabVertex[ (3*indices[2])+2 ] - m_TabVertex[ (3*indices[0])+2 ];

	produitVectoriel(Y, normal, B);
	normalise(B);
	// test position P / droite AC
	if( produitScalaire(B, Z) <= -dist )	// avant c'�tait 0.0
		return 1000.0f;

	// calcul de F = vecteur BC
	F[0] = Y[0] - X[0];
	F[1] = Y[1] - X[1];
	F[2] = Y[2] - X[2];

	// G = vecteur BP
	G[0] = Z[0]-X[0];
	G[1] = Z[1]-X[1];
	G[2] = Z[2]-X[2];

	produitVectoriel(F, normal, C);
	normalise(C);
	// test position P / droite BC
	if( produitScalaire(C, G)>= dist )
		return 1000.0f;

	return distanceW;
}

// Renvoie la distance entre le point de position 'pos' et le plus proche triangle de l'objet
// géo. N'effectue cette mesure que pour des distances inférieures à 'dist'
bool CSimpleGeo::checkContact( const float pos[3], float dist ) {
	float distanceW;

	if( TestContactPave( pos, dist ) ) {		// Teste proximité 'joueur / l'objet géo'
		for( int i=0; i<_numFaces; i++) {		//pour chaque triangle de l'objet géo.
			distanceW = testContactTriangle( i, pos, dist );
			if( distanceW <= dist ) {
				return true;
			}
		}
	}

	return false;
}

void CSimpleGeo::gereContactPlayer(float positionPlayer[3], CPlayer *player ) {
	if( _bSolid ) {	// Si l'objet est solide
		float dist = player->getRayon();	// Rayon de la sphère représentant le volume du joueur
		float distanceW;

		if( TestContactPave( positionPlayer, dist ) )	// Teste proximité 'joueur / l'objet géo' pour chaque triangle de l'objet géo
			for( int i=0; i<_numFaces; i++) {
				distanceW = testContactTriangle(i, positionPlayer, dist);

				if(distanceW < 500.0f) {	// Teste le contact avec le joueur (1.0f = valeur arbitraire mais grande)
					player->exeContactFunc( &_pNormalTriangle[3*i], distanceW );	// On a contact !
				}
			}
	}
}

bool CSimpleGeo::TestContactPave(const float pos[3], float dist) {
	// Teste si le point qui a pour position 'pos' se trouve ou non � une distance inférieure à 'dist'
	// du pavé englobant l'objet

	if( pos[0] < _maxX + dist )
		if( pos[1] < _maxY + dist )
			if( -pos[2] < _maxZ + dist )
				if( pos[0] > _minX - dist )
					if( pos[1] > _minY - dist )
						if( -pos[2] > _minZ - dist )
							return true;	// Le point 'pos' est à une distance inférieure

	return false;	// Le point 'pos' se trouve à une distance supérieure
}

float CSimpleGeo::gereLaserPlayer( float pos[3], CV3D& Dir, float dist) {
	if( !_bSolid )		// Si l'objet n'est pas solide, il ne peut être touché par le laser
		return dist;	// => on sort en renvoyant 'dist'

	float distanceVar;
	float *vertex, *normal;
	int *indices;
	CV3D AP, N, AB, AC, U, V, W, var;

	// Vérifie si le laser passe à proxomité de l'objet géo
	CV3D CP;	// Vecteur allant du point origine du laser au centre de l'objet g�o (=centre de la Bulle qui l'englobe)
	CP.X = _centre[0] - pos[0];
	CP.Y = _centre[1] - pos[1];
	CP.Z = _centre[2] + pos[2];

	float a = CP^Dir;				// Dir est normalis�
	float bCarre = CP^CP;			// b = norme du veteur CP au carr�
	float hCarre = bCarre - (a*a);	// b�=bCarre = h� + a� => h�=hCarre = b� - a�
	float rCarre = (0.001f+_rayon)*(0.001f+_rayon);
	if( hCarre > rCarre )			// Si distance (laser-centre de la sph�re) > rayon de la sphère
		return dist;				// Alors le rayon laser ne touche pas l'objet

	// Vérifie si le laser touche une face de la map
	vertex = m_TabVertex;
	for( int i=0; i<_numFaces; i++) { //pour chaque triangle de l'objet géo.
		indices = &m_TabFaces[3*i];
		normal = &_pNormalTriangle[3*i];

		// vecteur AP, P personnage, A point du triangle
		AP.X = pos[0]-vertex[ 3*indices[0] ];			//horizontal
		AP.Y = pos[1]-vertex[ (3*indices[0]) + 1 ];		//vertical
		AP.Z = -pos[2]-vertex[ (3*indices[0]) + 2 ];	//horizontal

		// normale au triangle
		N.X = normal[0];
		N.Y = normal[1];
		N.Z = normal[2];

		distanceVar = -(N^AP)/(N^Dir);

		if( distanceVar < 0.0f )	// Si la distance trouv�e pour ce triangle est n�gative
			continue;	// Passe au triangle suivant
		if( distanceVar > dist )	// Si elle est sup�rieure � une autre d�j� trouv�e
			continue;	// Passe au triangle suivant

		AB.X = vertex[ 3*indices[1] ] - vertex[ 3*indices[0] ];
		AB.Y = vertex[ (3*indices[1])+1 ] - vertex[ (3*indices[0])+1 ];
		AB.Z = vertex[ (3*indices[1])+2 ] - vertex[ (3*indices[0])+2 ];

		V = (Dir*distanceVar)+AP;
		var = AB*V;

		if( (N^var)<0.0f )	// Si le laser passe du mauvais c�t� du premier segment du triangle
			continue;	// Passe au triangle suivant

		AC.X = vertex[ 3*indices[2] ] - vertex[ 3*indices[0] ];
		AC.Y = vertex[ (3*indices[2])+1 ] - vertex[ (3*indices[0])+1 ];
		AC.Z = vertex[ (3*indices[2])+2 ] - vertex[ (3*indices[0])+2 ];

		var = AC*V;

		if( (N^var)>0.0f )	// S'il passe du mauvais c�t� du second segment du triangle
			continue;	// Passe au triangle suivant

		// Ici on est pass� du bon c�t� du second segment du triangle
		U = AC - AB;
		W = V - AB;

		var = W*U;

		if( (N^var)<=0.0f )	// S'il passe du bon c�t� du dernier segment du triangle
			dist = distanceVar;	// C'est qu'il le touche
	}

	return dist;	// Renvoie la distance trouvée
}

}	// JktMoteur
