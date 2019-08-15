
#include <string>
#include <fstream>
#include <set>
#include <cmath>
#include <map>
#include <sstream>

#include <GL/glew.h>

class CGame;

extern int JKT_RenderMode;

#include "util/Trace.h"
#include "spatial/XmlVocabulaire.h"
#include "util/Tableau.cpp"
#include "util/math_vectoriel.h"
#include "util/V3D.h"
#include "spatial/IfstreamMap.h"
#include "spatial/geo/TexVertexList.h"
#include <spatial/basic/Geometrical.h>
#include <spatial/basic/Refreshable.h>
#include "spatial/light/Light.h"
#include "spatial/Map.h"
#include "main/Player.h"
#include "spatial/contact.h"
#include "main/Cfg.h"
#include "spatial/materiau/MaterialMulti.h"

#include "spatial/geo/MultiMaterialGeo.h"

using namespace std;
using namespace jkt;

extern CCfg Config;

namespace jkt
{
class CGeoObject;
class CPorte;
class CNavette;

const char* CMultiMaterialGeo::identifier = "MultiMaterialGeo";

//CONSTRUCTEURS
CMultiMaterialGeo::CMultiMaterialGeo(CMap* map, const string& name, CMaterialMulti* mat, unsigned int nbrVertex, float* vertex,
		float* normals,	float* texvertex, std::map<int,int> &canauxnumbers, bool solid) : MapObject(map, MapObject::GEO) {
	m_NumVertex = 0;
	m_TabVectNormaux = NULL;
	m_pNormalTriangle = NULL;
	m_TabVertex = NULL;
	m_TabTexVertex = NULL;

	setName(name);						// Nom de l'objet
	setVertex(nbrVertex, vertex);		// Sommets
	setNormalVertex(normals);			// Vecteurs normaux
	setTexVertex(texvertex);			// Sommets de texture
	m_CanauxNumbers = canauxnumbers;	// Canaux de texture associ�s aux faces
	m_bSolid = solid;					// Objet solide ou �th�reux
	m_Material = mat;					// Materiau multiple

	cout << "\nCMultiMaterialGeo";
	for(int p=0 ; p<10 ; p++)
		cout << endl << m_TabVertex[p];
}

CMultiMaterialGeo::CMultiMaterialGeo(CMap* map) : MapObject(map, MapObject::GEO) {
	m_NumVertex = 0;
	m_TabVertex = NULL;			// Pointeur sur le tableau de sommets
	m_TabVectNormaux = NULL;
	m_TabTexVertex = NULL;
	m_bSolid = true;			// Objet solide par d�faut
	m_Material = NULL;
	m_pNormalTriangle = 0;		// Sera initialis� par Init()

	m_Rayon = 0.0;
	maxX = maxY = maxZ = minX = minY = minZ = 0.0f;
}

MapObject* CMultiMaterialGeo::clone() {
	return new CMultiMaterialGeo(*this);
}

void CMultiMaterialGeo::setVertex(int num, float *tab)
{
	if( m_TabVertex )	// Destruction de l'ancien tableau de sommets s'il existe
		delete[] m_TabVertex;

	m_NumVertex = num;	// Nombre de faces
	m_TabVertex = tab;
}

void CMultiMaterialGeo::setTexVertex(float *tab) {
	if( m_TabTexVertex )	// Destruction de l'ancien tableau de sommets s'il existe
		delete[] m_TabTexVertex;

	m_TabTexVertex = tab;
}

void CMultiMaterialGeo::init() throw(CErreur) {
	MinMax();			// Mesure les minimums et maximums de l'objet g�o
	Bulle();			// Mesure le centre et le rayon de la sph�re englobant l'objet g�o
	ConstruitBase();	// Construit la table des vecteurs normaux
}

void CMultiMaterialGeo::initGL() {
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
}

void CMultiMaterialGeo::freeGL() {
	glDeleteBuffers(VBO_BUFFER_SIZE, m_VboBufferNames);
}

void CMultiMaterialGeo::setNormalVertex(float *tab) {
	if(m_TabVectNormaux) {
		delete[] m_TabVectNormaux;
	}

	m_TabVectNormaux = tab;
}

void CMultiMaterialGeo::affiche() {
	glLineWidth( 1 );

	// Attachement VBO sommets
	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_VERTEX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	// Attachement VBO vecteurs normaux
	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_NORMAUX]);
	glNormalPointer(GL_FLOAT, 0, 0);

	// Attachement VBO sommets de texture
	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_TEXVERTEX]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	int start = 0;	// Index courant d'affichage de vertex

	// Affiche pour chaque canal avec le matériau spécifié
	map<int,int>::iterator iter = m_CanauxNumbers.begin();

	for( ; iter!=m_CanauxNumbers.end() ; iter++ ) {		// Dessine toutes les faces de la map
		int canal = iter->first;		// Numéro de sous-matériau
		int nbrFaces = iter->second;	// Nombre de faces concernées par ce canal

		// Récupération du sous-matériau courant
		CMaterial* material = m_Material->getMat(canal);

		if(material) {
			CMaterial::MAT_TYPE type = material->Type();

			// Initialisations de l'affichage en fonction du matériau
			if(type == CMaterial::MAT_TYPE_TEXTURE) {
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			else if(type == CMaterial::MAT_TYPE_SIMPLE) {
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			else {
				LOGWARN(("MultiMaterialGeo> Materiau inconnu"));
			}

			// Activation du matériau
			material->Active();

			// Affichage
			glDrawArrays(JKT_RenderMode, 3*start, 3*nbrFaces);

			// Désactivation du matériau
			material->Desactive();
		}
		else {
			LOGWARN(("MultiMaterialGeo> Matériau manquant"));
		}

		// Calcul de l'index courant d'affichage
		start += nbrFaces;
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if( Config.Debug.bAfficheNormaux ) {
		AfficheNormals();
	}
}

const char* CMultiMaterialGeo::toString() {
	ostringstream ttt;
	ttt << identifier << " Nom=" << getName() << " Mat=";

	if(m_Material)
		ttt << m_Material->toString();

	tostring = ttt.str();

	return tostring.c_str();
}

void CMultiMaterialGeo::afficheHighlighted(float r,float v,float b) {
	glLineWidth( 1 );

	// Attachement VBO sommets
	glBindBuffer(GL_ARRAY_BUFFER, m_VboBufferNames[VBO_VERTEX]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glColor3f(r, v, b);

	glEnableClientState(GL_VERTEX_ARRAY);

	int start = 0;	// Index courant d'affichage de vertex

	// Affiche pour chaque canal avec le matériau spécifié
	map<int,int>::iterator iter = m_CanauxNumbers.begin();

	for( ; iter!=m_CanauxNumbers.end() ; iter++ ) {			// Dessine toutes les faces de la map
		int nbrFaces = iter->second;	// Nombre de faces concernées par ce canal

		// Affichage
		glDrawArrays(JKT_RenderMode, 3*start, 3*nbrFaces);

		// Calcul de l'index courant d'affichage
		start += nbrFaces;
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	//AFFICHAGE DES VECTEURS NORMAUX
	if( Config.Debug.bAfficheNormaux )
		AfficheNormals();
}

void CMultiMaterialGeo::AfficheNormals() {
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

void CMultiMaterialGeo::setMaterial(const string& matRef) {
	MapObject* object = getMap()->getMapObjectByReference(matRef);

	if(!object) {
		stringstream txt;
		txt << "Erreur : Objet '" << matRef << "' introuvable";
		throw CErreur(txt);
	}

	CMaterialMulti* mat = dynamic_cast<CMaterialMulti*>(object);

	if(!mat) {
		stringstream txt;
		txt << "Erreur : L'objet '" << matRef << "' devrait être un CMultiMaterial";
		throw CErreur(txt);
	}

	m_Material = mat;
}

void CMultiMaterialGeo::MinMax() {
	minX = maxX = m_TabVertex[ 0 ];
	minY = maxY = m_TabVertex[ 1 ];
	minZ = maxZ = m_TabVertex[ 2 ];

	for( int i=1 ; i<m_NumVertex ; i++ ) {
		if( m_TabVertex[3*i] < minX )		//récupération des coordonnées du pavé englobant
			minX = m_TabVertex[3*i];		//l'objet géo
		if( m_TabVertex[(3*i)+1] < minY )
			minY = m_TabVertex[(3*i)+1];
		if( m_TabVertex[(3*i)+2] < minZ )
			minZ = m_TabVertex[(3*i)+2];

		if( m_TabVertex[3*i] > maxX )
			maxX = m_TabVertex[3*i];
		if( m_TabVertex[(3*i)+1] > maxY )
			maxY = m_TabVertex[(3*i)+1];
		if( m_TabVertex[(3*i)+2] > maxZ )
			maxZ = m_TabVertex[(3*i)+2];
	}
}

void CMultiMaterialGeo::Bulle() {
	float r0, r1, r2;
	// Calcul du centre de la sphère à partir des valeurs min/max
	m_Centre[0] = (minX+maxX)/2.0f;
	m_Centre[1] = (minY+maxY)/2.0f;
	m_Centre[2] = (minZ+maxZ)/2.0f;

	// Recherche du rayon de la sphère
	r0 = fabsf( minX-maxX );
	r1 = fabsf( minY-maxY );
	r2 = fabsf( minZ-maxZ );

	m_Rayon = sqrtf( (r0*r0) + (r1*r1) + (r2*r2) );
}

void CMultiMaterialGeo::ConstruitBase() {
	float X[3], Y[3];

	if( m_pNormalTriangle )
		delete[] m_pNormalTriangle;

	m_pNormalTriangle = new float[ 3*m_NumVertex ];

	for( int i=0; i<m_NumVertex; i=i+3 ) //pour chaque triangle de l'objet géo.
	{
		// X = vecteur coté AB
		X[0] = m_TabVertex[ (3*i)+3 ] - m_TabVertex[ 3*i ];
		X[1] = m_TabVertex[ (3*i)+4 ] - m_TabVertex[ 3*i+1 ];
		X[2] = m_TabVertex[ (3*i)+5 ] - m_TabVertex[ 3*i+2 ];

		// Y = vecteur coté AC
		Y[0] = m_TabVertex[ 3*i+6 ] - m_TabVertex[ 3*i ];
		Y[1] = m_TabVertex[ 3*i+7 ] - m_TabVertex[ 3*i+1 ];
		Y[2] = m_TabVertex[ 3*i+8 ] - m_TabVertex[ 3*i+2 ];

		produitVectoriel(X, Y, &m_pNormalTriangle[3*i]);	// calcul du vecteur normal au plan du triangle
		normalise( &m_pNormalTriangle[3*i] );		// normalise ce vecteur
	}
}

//DESTRUCTEUR
CMultiMaterialGeo::~CMultiMaterialGeo() {
	if( m_TabVertex ) {
		delete[] m_TabVertex;
		m_TabVertex = 0;
	}

	if( m_pNormalTriangle ) {
		delete[] m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs préliminaires � la gestion des contacts)
		m_pNormalTriangle = 0;
	}

	if( m_TabTexVertex ) {
		delete[] m_TabTexVertex;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs préliminaires � la gestion des contacts)
		m_TabTexVertex = 0;
	}
}

void CMultiMaterialGeo::echangeXY()	{	// Echange les axes X et Y de l'objet
	float varX, varY;

	for( int i=0 ; i<m_NumVertex ; i++ ) {
		varX = m_TabVertex[ 3*i ];
		varY = m_TabVertex[ (3*i)+1 ];

		m_TabVertex[ 3*i ] = varY;
		m_TabVertex[ (3*i)+1 ] = varX;
	}
}

void CMultiMaterialGeo::echangeXZ() {	// Echange les axes X et Y de l'objet
	float varX, varZ;

	for( int i=0 ; i<m_NumVertex ; i++ ) {
		varX = m_TabVertex[ 3*i ];
		varZ = m_TabVertex[ (3*i)+2 ];

		m_TabVertex[ 3*i ] = varZ;
		m_TabVertex[ (3*i)+2 ] =varX;
	}
}

void CMultiMaterialGeo::echangeYZ() {	// Echange les axes X et Y de l'objet
	float varY, varZ;

	for( int i=0 ; i<m_NumVertex ; i++ ) {
		varY = m_TabVertex[ (3*i)+1 ];
		varZ = m_TabVertex[ (3*i)+2 ];

		m_TabVertex[ (3*i)+1 ] = varZ;
		m_TabVertex[ (3*i)+2 ] =varY;
	}
}

void CMultiMaterialGeo::scale( float scaleX, float scaleY, float scaleZ ) {
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

void CMultiMaterialGeo::translate( float x, float y, float z ) {
	for( int i=0 ; i<m_NumVertex ; i++ ) {
		m_TabVertex[ (3*i) ] += x;
		m_TabVertex[ (3*i)+1 ] += y;
		m_TabVertex[ (3*i)+2 ] += z;
	}
}

bool CMultiMaterialGeo::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) throw(CErreur) {
	// Type
	const char* type = el->Value();

	if(!type)
		throw CErreur("Fichier Map corrompu CMultiMaterialGeo 2");

	if(strcmp(type, Xml::GEOMULTI))
		throw CErreur("Fichier Map corrompu CMultiMaterialGeo 3");

	// Nom
	const char* nom = el->Attribute(Xml::NOM);
	if(!nom)
		throw CErreur("Fichier Map corrompu CMultiMaterialGeo 1");
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

	// Solidité
	m_bSolid = Xml::LitSolidite(el);

	// Sommets
	m_TabVertex = CGeoMaker::LitVertex(el, m_NumVertex);

	// Vecteurs normaux
	m_TabVectNormaux = CGeoMaker::LitVecteursNormaux(el, m_NumVertex);

	// Sommets de texture
	m_TabTexVertex = CGeoMaker::LitTexVertex(el, m_NumVertex);

	// Sous-Matériaux de texture
	CGeoMaker::LitSousMateriaux(el, m_CanauxNumbers);

	return true;
}

bool CMultiMaterialGeo::Save(TiXmlElement* element) throw(CErreur) {
	// Sauve les données générales
	TiXmlElement* elGeo = new TiXmlElement(Xml::GEOMULTI);
	elGeo->SetAttribute(Xml::REF, getId());
	elGeo->SetAttribute(Xml::NOM, getName());
	if(isAbstract()) {
		elGeo->SetAttribute(Xml::ABSTRACT, "true");
	}
	element->LinkEndChild(elGeo);

	// Matériau
	CGeoMaker::SaveMateriau(elGeo, m_Material->getRef());

	// Matériau
	Xml::SaveElement(elGeo, Xml::MATERIAU, (int)m_Material->getRef());

	// Solidité
	CGeoMaker::SaveSolidite(elGeo, m_bSolid);

	// Sommets
	CGeoMaker::SaveVertex(elGeo, m_NumVertex, m_TabVertex);

	// Vecteurs normaux
	if(m_TabVectNormaux)
		CGeoMaker::SaveVecteursNormaux(elGeo, m_NumVertex, m_TabVectNormaux);

	// Sommets de texture
	CGeoMaker::SaveTexVertex(elGeo, m_NumVertex, m_TabTexVertex);

	// Canaux de texture
	CGeoMaker::SaveSousMateriaux(elGeo, m_CanauxNumbers);

	return true;
}

float CMultiMaterialGeo::testContactTriangle(unsigned int i, const float posPlayer[3], float dist) {		// Test s'il y a contact entre un point P et le i� triangle de l'objet géo
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
	if( produitScalaire(A, Z) >= dist )	// avant c'était 0.0
		return 1000.0f;

	Y[0] = m_TabVertex[ 3*i+6 ] - m_TabVertex[ 3*i+0 ];
	Y[1] = m_TabVertex[ 3*i+7 ] - m_TabVertex[ 3*i+1 ];
	Y[2] = m_TabVertex[ 3*i+8 ] - m_TabVertex[ 3*i+2 ];

	produitVectoriel(Y, normal, B);
	normalise(B);
	// test position P / droite AC
	if( produitScalaire(B, Z) <= -dist )	// avant c'était 0.0
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
// géo. N'effectue cette mesure que pour des distances inférieures � 'dist'
bool CMultiMaterialGeo::checkContact( const float pos[3], float dist ) {
	float distanceW;
	if( TestContactPave( pos, dist ) )	// Teste proximité 'joueur / l'objet géo'
	{
		for( int i=0; i<m_NumVertex; i=i+3)		//pour chaque triangle de l'objet géo.
		{
			distanceW = testContactTriangle( i, pos, dist );
			if( distanceW <= dist )
				return true;
		}
	}

	return false;
}

void CMultiMaterialGeo::gereContactPlayer(float positionPlayer[3], CPlayer *player) {
	float dist = player->getRayon();	// Rayon de la sph�re représentant le volume du joueur
	float distanceW;

	if( m_bSolid )	// Si l'objet est solide
		if( TestContactPave( positionPlayer, dist ) )	// Teste proximité 'joueur / l'objet géo'
			for( int i=0; i<m_NumVertex; i=i+3)		//pour chaque triangle de l'objet géo.
			{
				distanceW = testContactTriangle( i, positionPlayer, dist );

				if( distanceW<500.0f ) // Teste le contact avec le joueur (1.0f = valeur arbitraire mais grande)
					player->exeContactFunc(&m_pNormalTriangle[3*i], distanceW);	// On a contact !
			}
}

bool CMultiMaterialGeo::TestContactPave( const float pos[3], float dist ) {
	// Teste si le point qui a pour position 'pos' se trouve ou non � une distance inférieure à 'dist'
	// du pavé englobant l'objet

	if( pos[0] < maxX+dist )
		if( pos[1] < maxY+dist )
			if( -pos[2] < maxZ+dist )
				if( pos[0] > minX-dist )
					if( pos[1] > minY-dist )
						if( -pos[2] > minZ-dist )
							return true;	// Le point 'pos' est à une distance inférieure

	return false;	// Le point 'pos' se trouve à une distance supérieure
}

float CMultiMaterialGeo::gereLaserPlayer( float pos[3], CV3D &Dir, float dist) {
	float distanceVar;
	float *vertex, *normal;
	CV3D AP, N, AB, AC, U, V, W, var;

	if( !m_bSolid )	// Si l'objet n'est pas solide, il ne peut �tre touché par le laser
		return dist;	// => on sort en renvoyant 'dist'

	// Vérifie si le laser passe � proxomité de l'objet géo
	CV3D CP;	// Vecteur allant du point origine du laser au centre de l'objet géo (=centre de la Bulle qui l'englobe)
	CP.X = m_Centre[0] - pos[0];
	CP.Y = m_Centre[1] - pos[1];
	CP.Z = m_Centre[2] + pos[2];

	float a = CP^Dir;				// Dir est normalisé
	float bCarre = CP^CP;			// b = norme du veteur CP au carré
	float hCarre = bCarre - (a*a);	// b�=bCarre = h� + a� => h�=hCarre = b� - a�
	float rCarre = (0.001f+m_Rayon)*(0.001f+m_Rayon);
	if( hCarre > rCarre )			// Si distance (laser-centre de la sph�re) > rayon de la sph�re
		return dist;				// Alors le rayon laser ne touche pas l'objet

	// V�rifie si le laser touche une face de la map
	vertex = m_TabVertex;

	for( int i=0; i<m_NumVertex; i=i+3) {		//pour chaque triangle de l'objet g�o.
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
