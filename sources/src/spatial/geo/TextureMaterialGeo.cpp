
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

class CGame;

extern int JKT_RenderMode;

#include "util/Trace.h"
#include "spatial/XmlVocabulaire.h"
#include "util/Tableau.cpp"
#include "util/math_vectoriel.h"
#include "util/V3D.h"
#include "spatial/IfstreamMap.h"
#include <spatial/basic/Geometrical.h>
#include <spatial/basic/Refreshable.h>
#include "spatial/light/Light.h"
#include "spatial/Map.h"
#include "main/Player.h"
#include "spatial/contact.h"
#include "main/Cfg.h"
#include "spatial/materiau/MaterialTexture.h"

#include "spatial/geo/TextureMaterialGeo.h"

using namespace std;
using namespace jkt;

extern CCfg Config;

namespace jkt
{
class CGeoObject;
class CPorte;
class CNavette;

const char* CTextureMaterialGeo::identifier = "TextureMaterialGeo";

//CONSTRUCTEURS
CTextureMaterialGeo::CTextureMaterialGeo(CMap* map, const string& name, CMaterialTexture* mat,
		unsigned int nbrvertex, float* vertex, float* normals, float* texvertex, bool solid) : MapObject(map, MapObject::GEO) {
	m_NumVertex = 0;
	m_TabVectNormaux = 0;
	m_pNormalTriangle = 0;		// Sera initialis� par Init()
	m_TabVertex = 0;			// Pointeur sur le tableau de sommets
	m_TabTexVertex = 0;

	setName(name);	// Nom de l'objet
	setVertex(nbrvertex, vertex);	// Sommets
	setNormalVertex(normals);
	m_bSolid = solid;				// Objet solide ou �th�reux
	m_Material = mat;
	m_TabTexVertex = texvertex;
	_isGlInitialized = false;
}

CTextureMaterialGeo::CTextureMaterialGeo(CMap* map) : MapObject(map, MapObject::GEO) {
	m_NumVertex = 0;
	m_TabVertex = NULL;			// Pointeur sur le tableau de sommets
	m_TabVectNormaux = NULL;
	m_bSolid = true;			// Objet solide par d�faut
	m_Material = NULL;
	m_TabTexVertex = NULL;
	m_pNormalTriangle = NULL;		// Sera initialis� par Init()
	_maxX = _maxY = _maxZ = _minX = _minY = _minZ = 0.0f;
	m_Rayon = 0.0f;
	_isGlInitialized = false;
}

CTextureMaterialGeo::CTextureMaterialGeo(const CTextureMaterialGeo& other) : MapObject(other) {
	tostring = other.tostring;

	// Bulle
	_minX = other._minX;
	_minY = other._minY;
	_minZ = other._minZ;
	_maxX = other._maxX;
	_maxY = other._maxY;
	_maxZ = other._maxZ;

	// Centre
	m_Centre[0] = other.m_Centre[0];
	m_Centre[1] = other.m_Centre[1];
	m_Centre[2] = other.m_Centre[2];

	// Rayon
	m_Rayon = other.m_Rayon;

	// Solidit�
	m_bSolid = other.m_bSolid;

	// Nombres
	m_NumVertex = other.m_NumVertex;

	// Sommets
	setVertex(other.m_NumVertex, other.m_TabVertex);

	// Normales
	m_pNormalTriangle = 0;

	if(other.m_pNormalTriangle) {
		m_pNormalTriangle = new float[ 3*m_NumVertex ];

		for(int i=0 ; i<3*m_NumVertex ; i++) {
			m_pNormalTriangle[i] = other.m_pNormalTriangle[i];
		}
	}

	_isGlInitialized = false;
}

MapObject* CTextureMaterialGeo::clone() {
	return new CTextureMaterialGeo(*this);
}
void CTextureMaterialGeo::setVertex(int numVertex, float *tab) {
	if( m_TabVertex )	// Destruction de l'ancien tableau de sommets s'il existe
		delete[] m_TabVertex;

	m_NumVertex = numVertex;	// Nombre de faces
	m_TabVertex = tab;
}

void CTextureMaterialGeo::init() throw(CErreur) {
	MinMax();			// Mesure les minimums et maximums de l'objet g�o
	Bulle();			// Mesure le centre et le rayon de la sph�re englobant l'objet g�o
	ConstruitBase();	// Construit la table des vecteurs normaux
}

void CTextureMaterialGeo::initGL() {
	glGenBuffers(VBO_BUFFER_SIZE, m_VboBufferNames);

	// Sommets
	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, m_NumVertex*3*sizeof(float), m_TabVertex, GL_STATIC_DRAW);

	// Vecteurs normaux
	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_NORMAUX]);
	glBufferData(GL_ARRAY_BUFFER, m_NumVertex*3*sizeof(float), m_TabVectNormaux, GL_STATIC_DRAW);

	// Sommets de texture
	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_TEXVERTEX]);
	glBufferData(GL_ARRAY_BUFFER, m_NumVertex*2*sizeof(float), m_TabTexVertex, GL_STATIC_DRAW);

	_isGlInitialized = true;
}

void CTextureMaterialGeo::freeGL() {
	LOGINFO(("JAMAIS"));
	glDeleteBuffers(VBO_BUFFER_SIZE, m_VboBufferNames);
}

void CTextureMaterialGeo::setNormalVertex(float *tab) {
	if(m_TabVectNormaux) {
		delete[] m_TabVectNormaux;
	}

	m_TabVectNormaux = tab;
}

void CTextureMaterialGeo::affiche() {
	_isGlInitialized = true;

	if(!_isGlInitialized) {
		LOGERROR(("Affichage avant initialisation GL"));
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_VERTEX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_NORMAUX]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_TEXVERTEX]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	m_Material->Active();

	glLineWidth( 1 );

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glDrawArrays(JKT_RenderMode, 0, m_NumVertex);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_Material->Desactive();

	glDisable( GL_TEXTURE_2D );

	//AFFICHAGE DES VECTEURS NORMAUX
	if( Config.Debug.bAfficheNormaux )
		AfficheNormals();
}

void CTextureMaterialGeo::afficheHighlighted(float r,float v,float b) {
	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_VERTEX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glLineWidth( 1 );

	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(r, v, b); // D�finit la couleur de l'objet g�o. s�lectionn�

	glDrawArrays(JKT_RenderMode, 0, m_NumVertex);

	glDisableClientState(GL_VERTEX_ARRAY);

	//AFFICHAGE DES VECTEURS NORMAUX
	if( Config.Debug.bAfficheNormaux )
		AfficheNormals();
}

const char* CTextureMaterialGeo::toString()
{
	ostringstream ttt;
	ttt << identifier << " Nom=" << getName() << " Mat=";

	if(m_Material)
		ttt << m_Material->toString();

	tostring = ttt.str();

	return tostring.c_str();
}

void CTextureMaterialGeo::AfficheNormals() {
	float pos[3], bout[ 3 ];
	float facteur = 0.05f;

	if( m_TabVectNormaux ) {
		glBegin( GL_LINES );

		for( int i=0; i < m_NumVertex ; i++ ) //dessine toutes les faces de la map
		{
			glColor3f( 1.0f, 0.0f, 0.0f );
			pos[ 0 ] = m_TabVertex[ i*3 ];
			pos[ 1 ] = m_TabVertex[ i*3 + 1 ];
			pos[ 2 ] = m_TabVertex[ i*3 + 2 ];

			bout[ 0 ] = (m_TabVectNormaux[ (9*i)+0 ]*facteur) + pos[ 0 ];
			bout[ 1 ] = (m_TabVectNormaux[ (9*i)+1 ]*facteur) + pos[ 1 ];
			bout[ 2 ] = (m_TabVectNormaux[ (9*i)+2 ]*facteur) + pos[ 2 ];

			glVertex3fv( pos );
			glVertex3fv( bout );
		}
		glEnd();
	}
}

void CTextureMaterialGeo::setMaterial(const string& matRef) {
	MapObject* object = getMap()->getMapObjectByReference(matRef);

	if(!object) {
		stringstream txt;
		txt << "Erreur : Objet '" << matRef << "' introuvable";
		throw CErreur(txt);
	}

	CMaterialTexture* mat = dynamic_cast<CMaterialTexture*>(object);

	if(!mat) {
		stringstream txt;
		txt << "Erreur : L'objet '" << matRef << "' devrait être un CMaterialTexture";
		throw CErreur(txt);
	}

	m_Material = mat;
}

void CTextureMaterialGeo::MinMax()
{
	_minX = _maxX = m_TabVertex[ 0 ];
	_minY = _maxY = m_TabVertex[ 1 ];
	_minZ = _maxZ = m_TabVertex[ 2 ];

	for( int i=1 ; i<m_NumVertex ; i++ )
	{
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

void CTextureMaterialGeo::Bulle()
{
	float r0, r1, r2;
	// Calcul du centre de la sph�re � partir des valeurs min/max
	m_Centre[0] = (_minX+_maxX)/2.0f;
	m_Centre[1] = (_minY+_maxY)/2.0f;
	m_Centre[2] = (_minZ+_maxZ)/2.0f;

	// Recherche du rayon de la sph�re
	r0 = fabsf( _minX-_maxX );
	r1 = fabsf( _minY-_maxY );
	r2 = fabsf( _minZ-_maxZ );

	m_Rayon = sqrtf( (r0*r0) + (r1*r1) + (r2*r2) );
}

void CTextureMaterialGeo::ConstruitBase()
{
	float X[3], Y[3];

	if( m_pNormalTriangle )
		delete[] m_pNormalTriangle;

	m_pNormalTriangle = new float[ 3*m_NumVertex ];

	for( int i=0; i<m_NumVertex; i=i+3 ) //pour chaque triangle de l'objet g�o.
	{
		// X = vecteur cot� AB
		X[0] = m_TabVertex[ (3*i)+3 ] - m_TabVertex[ 3*i ];
		X[1] = m_TabVertex[ (3*i)+4 ] - m_TabVertex[ 3*i+1 ];
		X[2] = m_TabVertex[ (3*i)+5 ] - m_TabVertex[ 3*i+2 ];

		// Y = vecteur cot� AC
		Y[0] = m_TabVertex[ 3*i+6 ] - m_TabVertex[ 3*i ];
		Y[1] = m_TabVertex[ 3*i+7 ] - m_TabVertex[ 3*i+1 ];
		Y[2] = m_TabVertex[ 3*i+8 ] - m_TabVertex[ 3*i+2 ];

		produitVectoriel(X, Y, &m_pNormalTriangle[3*i]);	// calcul du vecteur normal au plan
		// du triangle
		normalise( &m_pNormalTriangle[3*i] );		// normalise ce vecteur
	}
}

//DESTRUCTEUR
CTextureMaterialGeo::~CTextureMaterialGeo() {
	LOGINFO(("JAMAIS"));

	if( m_TabVertex ) {
		delete[] m_TabVertex;
		m_TabVertex = 0;
	}

	if( m_TabTexVertex ) {
		delete[] m_TabTexVertex;
		m_TabTexVertex = 0;
	}

	if( m_pNormalTriangle ) LOGINFO(("JAMAIS"));{
		delete[] m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs pr�liminaires � la gestion des contacts)
		m_pNormalTriangle = 0;
	}
}

void CTextureMaterialGeo::echangeXY()	// Echange les axes X et Y de l'objet
{
	float varX, varY;

	for( int i=0 ; i<m_NumVertex ; i++ )
	{
		varX = m_TabVertex[ 3*i ];
		varY = m_TabVertex[ (3*i)+1 ];

		m_TabVertex[ 3*i ] = varY;
		m_TabVertex[ (3*i)+1 ] = varX;
	}
}

void CTextureMaterialGeo::echangeXZ() {	// Echange les axes X et Y de l'objet
	float varX, varZ;

	for( int i=0 ; i<m_NumVertex ; i++ ) {
		varX = m_TabVertex[ 3*i ];
		varZ = m_TabVertex[ (3*i)+2 ];

		m_TabVertex[ 3*i ] = varZ;
		m_TabVertex[ (3*i)+2 ] =varX;
	}
}

void CTextureMaterialGeo::echangeYZ() {	// Echange les axes X et Y de l'objet
	float varY, varZ;

	for( int i=0 ; i<m_NumVertex ; i++ ) {
		varY = m_TabVertex[ (3*i)+1 ];
		varZ = m_TabVertex[ (3*i)+2 ];

		m_TabVertex[ (3*i)+1 ] = varZ;
		m_TabVertex[ (3*i)+2 ] =varY;
	}
}

void CTextureMaterialGeo::scale(float scaleX, float scaleY, float scaleZ) {
	for( int i=0 ; i<m_NumVertex ; i++ ) {
		m_TabVertex[ (3*i) ] *= scaleX;
		m_TabVertex[ (3*i)+1 ] *= scaleY;
		m_TabVertex[ (3*i)+2 ] *= scaleZ;
	}

	if( m_TabVectNormaux ) {
		for( int i=0 ; i<(m_NumVertex) ; i++ ) {
			m_TabVectNormaux[ (3*i)+0 ] *= scaleX;
			m_TabVectNormaux[ (3*i)+1 ] *= scaleY;
			m_TabVectNormaux[ (3*i)+2 ] *= scaleZ;

			normalise( &m_TabVectNormaux[ 3*i ] );
		}
	}
}

void CTextureMaterialGeo::translate(float x, float y, float z) {
	for( int i=0 ; i<m_NumVertex ; i++ ) {
		m_TabVertex[ (3*i)+0 ] += x;
		m_TabVertex[ (3*i)+1 ] += y;
		m_TabVertex[ (3*i)+2 ] += z;
	}
}

bool CTextureMaterialGeo::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) throw(CErreur) {
	// Type
	const char* type = el->Value();

	if(!type)
		throw CErreur("Fichier Map corrompu CTextureMaterialGeo 2");

	if(strcmp(type, Xml::GEOTEXTURE)) {
		throw CErreur("Fichier Map corrompu CTextureMaterialGeo 3");
	}

	// Nom
	const char* nom = el->Attribute(Xml::NOM);
	if(!nom)
		throw CErreur("Fichier Map corrompu CTextureMaterialGeo 1");
	setName(nom);

	// Abstract
	const char* attr = el->Attribute(Xml::ABSTRACT);
	bool abstract = Xml::LitBooleanNotMandatory(attr, false);
	setAbstract(abstract);

	// Référence
	const char* reference = el->Attribute(Xml::REF);

	if(reference) {
		_reference = reference;
	}

	// Materiau
	string refMat = Xml::LitMaterialRef(el);
	setMaterial(refMat);

	// Solidit�
	m_bSolid = Xml::LitSolidite(el);

	// Sommets
	m_TabVertex = CGeoMaker::LitVertex(el, m_NumVertex);

	// Vecteurs normaux
	m_TabVectNormaux = CGeoMaker::LitVecteursNormaux(el, m_NumVertex);

	// Sommets de texture
	m_TabTexVertex = CGeoMaker::LitTexVertex(el, m_NumVertex);

	return true;
}

bool CTextureMaterialGeo::Save(TiXmlElement* element) throw(CErreur) {
	// Sauve les données générales
	TiXmlElement* elGeo = new TiXmlElement(Xml::GEOTEXTURE);
	elGeo->SetAttribute(Xml::REF, getId());
	elGeo->SetAttribute(Xml::NOM, getName());
	if(isAbstract()) {
		elGeo->SetAttribute(Xml::ABSTRACT, "true");
	}
	element->LinkEndChild(elGeo);

	// Mat�riau
	CGeoMaker::SaveMateriau(elGeo, m_Material->getRef());

	// Solidit�
	CGeoMaker::SaveSolidite(elGeo, m_bSolid);

	// Sommets
	CGeoMaker::SaveVertex(elGeo, m_NumVertex, m_TabVertex);

	// Sommets de texture
	CGeoMaker::SaveTexVertex(elGeo, m_NumVertex, m_TabTexVertex);

	// Vecteurs normaux
	if(m_TabVectNormaux)
		CGeoMaker::SaveVecteursNormaux(elGeo, m_NumVertex, m_TabVectNormaux);

	return true;
}

float CTextureMaterialGeo::testContactTriangle( unsigned int i, const float posPlayer[3], float dist ) {		// Test s'il y a contact entre un point P et le i� triangle de l'objet g�o
	float *normal = &m_pNormalTriangle[3*i];
	float A[3], B[3], C[3], F[3], G[3], X[3], Y[3], Z[3];
	float distanceW;

	// Z = vecteur AP, P personnage
	Z[0] =  posPlayer[0] - m_TabVertex[ 3*i+0 ];
	Z[1] =  posPlayer[1] - m_TabVertex[ 3*i+1 ];
	Z[2] = -posPlayer[2] - m_TabVertex[ 3*i+2 ];

	distanceW = produitScalaire(normal, Z); //distance entre le plan du triangle et le joueur
	if( distanceW >= dist )
		return 1000.0f;		// Y'a pas contact

	if( distanceW <= - dist )
		return 1000.0f;		// Y'a pas contact

	X[0] = m_TabVertex[ 3*i+3 ] - m_TabVertex[ 3*i+0 ];
	X[1] = m_TabVertex[ 3*i+4 ] - m_TabVertex[ 3*i+1 ];
	X[2] = m_TabVertex[ 3*i+5 ] - m_TabVertex[ 3*i+2 ];

	produitVectoriel( X, normal, A );
	normalise( A );
	// test position P / droite AB
	if( produitScalaire(A, Z) >= dist )	// avant c'�tait 0.0
		return 1000.0f;

	Y[0] = m_TabVertex[ 3*i+6 ] - m_TabVertex[ 3*i+0 ];
	Y[1] = m_TabVertex[ 3*i+7 ] - m_TabVertex[ 3*i+1 ];
	Y[2] = m_TabVertex[ 3*i+8 ] - m_TabVertex[ 3*i+2 ];

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
// g�o. N'effectue cette mesure que pour des distances inf�rieures � 'dist'
bool CTextureMaterialGeo::checkContact( const float pos[3], float dist ) {
	float distanceW;
	if( TestContactPave( pos, dist ) )	// Teste proximit� 'joueur / l'objet g�o'
	{
		for( int i=0; i<m_NumVertex; i=i+3)		//pour chaque triangle de l'objet g�o.
		{
			distanceW = testContactTriangle( i, pos, dist );
			if( distanceW <= dist )
				return true;
		}
	}

	return false;
}

void CTextureMaterialGeo::gereContactPlayer(float positionPlayer[3], CPlayer *player) {
	float dist = player->getRayon();	// Rayon de la sph�re repr�sentant le volume du joueur
	float distanceW;

	if( m_bSolid )	// Si l'objet est solide
		if( TestContactPave( positionPlayer, dist ) )	// Teste proximité 'joueur / l'objet géo'
			for( int i=0; i<m_NumVertex; i=i+3) {		//pour chaque triangle de l'objet géo.
				distanceW = testContactTriangle( i, positionPlayer, dist );

				if( distanceW<500.0f ) // Teste le contact avec le joueur (1.0f = valeur arbitraire mais grande)
					player->exeContactFunc(&m_pNormalTriangle[3*i], distanceW);	// On a contact !
			}
}

bool CTextureMaterialGeo::TestContactPave( const float pos[3], float dist ) {
	// Teste si le point qui a pour position 'pos' se trouve ou non � une distance inf�rieure � 'dist'
	// du pav� englobant l'objet

	if( pos[0] < _maxX+dist )
		if( pos[1] < _maxY+dist )
			if( -pos[2] < _maxZ+dist )
				if( pos[0] > _minX-dist )
					if( pos[1] > _minY-dist )
						if( -pos[2] > _minZ-dist )
							return true;	// Le point 'pos' est � une distance inf�rieure

	return false;	// Le point 'pos' se trouve � une distance sup�rieure
}

float CTextureMaterialGeo::gereLaserPlayer( float pos[3], CV3D &Dir, float dist) {
	float distanceVar;
	float *vertex, *normal;
	CV3D AP, N, AB, AC, U, V, W, var;

	if( !m_bSolid )	// Si l'objet n'est pas solide, il ne peut �tre touch� par le laser
		return dist;	// => on sort en renvoyant 'dist'

	// V�rifie si le laser passe � proxomit� de l'objet g�o
	CV3D CP;	// Vecteur allant du point origine du laser au centre de l'objet g�o (=centre de la Bulle qui l'englobe)
	CP.X = m_Centre[0] - pos[0];
	CP.Y = m_Centre[1] - pos[1];
	CP.Z = m_Centre[2] + pos[2];

	float a = CP^Dir;				// Dir est normalis�
	float bCarre = CP^CP;			// b = norme du veteur CP au carr�
	float hCarre = bCarre - (a*a);	// b�=bCarre = h� + a� => h�=hCarre = b� - a�
	float rCarre = (0.001f+m_Rayon)*(0.001f+m_Rayon);
	if( hCarre > rCarre )			// Si distance (laser-centre de la sph�re) > rayon de la sph�re
		return dist;				// Alors le rayon laser ne touche pas l'objet

	// V�rifie si le laser touche une face de la map
	vertex = m_TabVertex;
	for( int i=0; i<m_NumVertex; i=i+3) //pour chaque triangle de l'objet g�o.
	{
		normal = &m_pNormalTriangle[3*i];

		// vecteur AP, P personnage, A point du triangle
		AP.X = pos[0]- vertex[ 3*i ];	//horizontal
		AP.Y = pos[1]- vertex[ 3*i+1 ];	//vertical
		AP.Z = -pos[2]-vertex[ 3*i+2 ];	//horizontal

		// normale au triangle
		N.X = normal[0];
		N.Y = normal[1];
		N.Z = normal[2];

		distanceVar = -(N^AP)/(N^Dir);

		if( distanceVar < 0.0f )	// Si la distance trouv�e pour ce triangle est n�gative
			continue;	// Passe au triangle suivant
		if( distanceVar > dist )	// Si elle est sup�rieure � une autre d�j� trouv�e
			continue;	// Passe au triangle suivant

		AB.X = vertex[ 3*i+3 ] - vertex[ 3*i ];
		AB.Y = vertex[ 3*i+4 ] - vertex[ 3*i+1 ];
		AB.Z = vertex[ 3*i+5 ] - vertex[ 3*i+2 ];

		V = (Dir*distanceVar)+AP;
		var = AB*V;

		if( (N^var)<0.0f )	// Si le laser passe du mauvais c�t� du premier segment du triangle
			continue;	// Passe au triangle suivant

		AC.X = vertex[ 3*i+6 ] - vertex[ 3*i ];
		AC.Y = vertex[ 3*i+7 ] - vertex[ 3*i+1 ];
		AC.Z = vertex[ 3*i+8 ] - vertex[ 3*i+2 ];

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

	return dist;	// Renvoie la distance trouv�e
}

}	// JktMoteur
