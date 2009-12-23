

/****************************************************************/
/*																*/
/*		Destiné à lire les fichiers ASE générés par 3DSMAX		*/
/*			Il faut se méfier du code de fichier.cpp			*/
/*				car il est extrêment instable					*/
/*				Méritetait d'être réécrit						*/
/*																*/
/*		Codé par JC												*/
/*																*/
/****************************************************************/

#include <fstream>
#include <iostream>
#include <string>
#include <set>

using namespace std;

#include "GeoObject.h"
#include "Light.h"
#include "Tableau.cpp"
#include "GeoMaterial.h"
#include "Map.h"
#include "Cfg.h"

#include "tableau.cpp"

#include "fichier.h"

static const basic_string <char>::size_type npos = -1;

#define fScale (300.0f)
extern CCfg Config;

bool bAffiche;


bool LitFaces( ifstream &from, unsigned int num, unsigned int* tabIndex )
{
	bool resul;
	char mot_MESH_FACE[] = "*MESH_FACE";
	unsigned int index1, index2, index3;

	string mot;

	if( bAffiche )
		cout << "\nTABLEAU D'INDEX :\n";

	for( unsigned int i=0 ; i<num ; i++)
	{
		resul = false;
		while( !resul )		// Recherche du "*MESH_FACE" pour début de ligne de tableau
		{									
			if( !(from>>mot) )	// Si on atteint la fin ou qu'il y a un problème avec le fichier
			{
				cerr << "\nErreur (fichier::LitFaces) : Fichier ASE corrompu";
				return false;	// La lecture n'a pas abouti
			}
			if( mot==mot_MESH_FACE )
				resul = true;
		}
		
		from >> mot >> mot;
		from >> index1 >> mot >> index2 >> mot >> index3;
		
		tabIndex[(3*i)] = index1;
		tabIndex[(3*i)+1] = index2;
		tabIndex[(3*i)+2] = index3;

		if( bAffiche )
			cout << "\n\t" << index1 << index2 << index3;
	}

	return true;	// Tout s'est bien passé
}


bool LitVertex( ifstream &from, CGeoObject *pGeoObject )
{
	bool resul;
	char mot_MESH_VERTEX[] = "*MESH_VERTEX";
	float *tabVertex = pGeoObject->m_TabVertex;
	unsigned int num = pGeoObject->m_NumVertex;

	string mot;

	unsigned int index;
	float vertex1, vertex2, vertex3;

	if( bAffiche )
		cout << "\nTABLEAU DE SOMMETS :\n";
	
	for( unsigned int i=0 ; i<num ; i++)
	{
		resul = false;
		while( !resul )		// Recherche du "*MESH_FACE" pour début de ligne de tableau
		{						
			if( !(from>>mot) )	// Si on atteint la fin ou qu'il y a un problème avec le fichier
			{
				cerr << "\nErreur (fichier::LitVertex) : Fichier ASE corrompu";
				return false;	// La lecture n'a pas abouti
			}
			if( mot==mot_MESH_VERTEX )
				resul = true;
		}

		from >> index >> vertex1 >> vertex2 >> vertex3;
		tabVertex[3*i] = vertex1/fScale;
		tabVertex[(3*i)+1] = vertex2/fScale;
		tabVertex[(3*i)+2] = vertex3/fScale;
		
		if( bAffiche )
			cout <<"\n\t" <<index<<' '<<tabVertex[3*i]<<' '<<tabVertex[3*i+1]<<' '<< tabVertex[3*i+2];
	}
	
	return true;	// Tout s'est bien passé
}

bool LitTexVertex( ifstream &from, CGeoObject *geo )
{
	bool resul;
	char mot_MESH_TVERT[] = "*MESH_TVERT";
	unsigned int num = geo->m_NumTexVertex;
	float *tabTexVertex;

	string mot;

	geo->m_TabTexVertex = new float[3*num];
	tabTexVertex = geo->m_TabTexVertex;

	unsigned int index;

	if( bAffiche )
		cout << "\nTABLEAU DE SOMMETS DE TEXTURES :\n";

	for( unsigned int i=0 ; i<num ; i++)
	{
		resul = false;
		while( !resul )		// Recherche du "*MESH_FACE" pour début de ligne de tableau
		{										
			if( !(from>>mot) )	// Si on atteint la fin ou qu'il y a un problème avec le fichier
			{
				cerr << "\nErreur (fichier::LitTexVertex) : Fichier ASE corrompu";
				return false;	// La lecture n'a pas abouti
			}

			if( mot==mot_MESH_TVERT )
				resul = true;
		}

		from >> index >> tabTexVertex[(3*i)+1] >> tabTexVertex[3*i] >> tabTexVertex[(3*i)+2];
		
		if( bAffiche )
			cout<<"\n\t"<<index<<' '<<tabTexVertex[3*i]<<' '<<tabTexVertex[(3*i)+1]<<' '<<tabTexVertex[(3*i)+2];
	}

	return true;	// Tout s'est bien passé
}


bool LitNormaux( ifstream &from, CGeoObject *geo )
{
	bool resul;
	char mot_MESH_FACENORMAL[] = "*MESH_FACENORMAL";
	unsigned int num = geo->m_NumFaces;
	float *tabVectNormaux;

	string mot;

	geo->m_TabVectNormaux = new float[ 9*num ];
	tabVectNormaux = geo->m_TabVectNormaux;

	unsigned int index, index1, index2, index3;
	float normal1, normal2, normal3;
	float normal11, normal21, normal31;
	float normal12, normal22, normal32;
	float normal13, normal23, normal33;

	if( bAffiche )
		cout << "\nTABLEAU DE VECTEURS NORMAUX :\n";

	for( unsigned int i=0 ; i<num ; i++)
	{
		resul = false;
		while( !resul )		// Recherche du "*MESH_FACE" pour début de ligne de tableau
		{
			if( !(from>>mot) )	// Si on atteint la fin ou qu'il y a un problème avec le fichier
			{
				cerr << "\nErreur (fichier::LitNormaux) : Fichier ASE corrompu";
				return false;	// La lecture n'a pas abouti
			}

			if( mot==mot_MESH_FACENORMAL )
				resul = true;
		}
		
		from >> index >> normal1 >> normal2 >> normal3;		//CETTE LIGNE NE MARCHE PAS
		
		if( bAffiche )
			cout << "\n" << index << normal1 << normal2 << normal3;
				
		from >> mot >> index1 >> normal11 >> normal21 >> normal31;
		tabVectNormaux[9*i] = normal11;
		tabVectNormaux[(9*i)+1] = normal21;
		tabVectNormaux[(9*i)+2] = normal31;
		
		from >> mot >> index2 >> normal12 >> normal22 >> normal32;
		tabVectNormaux[9*i+3] = normal12;
		tabVectNormaux[(9*i)+4] = normal22;
		tabVectNormaux[(9*i)+5] = normal32;
		
		from >> mot >> index3 >> normal13 >> normal23 >> normal33;
		tabVectNormaux[9*i+6] = normal13;
		tabVectNormaux[(9*i)+7] = normal23;
		tabVectNormaux[(9*i)+8] = normal33;

		if( bAffiche )
		{
			cout<<"\n"<<index1<<' '<<tabVectNormaux[9*i]<<' '<<tabVectNormaux[9*i+1]<<' '<<tabVectNormaux[9*i+2];
			cout<<"\n"<<index2<<' '<<tabVectNormaux[9*i+3]<<' '<<tabVectNormaux[9*i+4]<<' '<<tabVectNormaux[9*i+5];
			cout<<"\n"<<index3<<' '<<tabVectNormaux[9*i+6]<<' '<<tabVectNormaux[9*i+7]<<' '<<tabVectNormaux[9*i+8];
		}
	}

	return true;	// Tout s'est bien passé
}

bool LitTexFaces( ifstream &from, CGeoObject *geo )
{
	bool resul;
	char mot_MESH_TFACE[] = "*MESH_TFACE";
	unsigned int num = geo->m_NumTexFaces;
	unsigned *tabTexFaces;
	unsigned int index;
	unsigned int tface1, tface2, tface3;

	string mot;

	geo->m_TabTexFaces = new unsigned int[3*num];
	tabTexFaces = geo->m_TabTexFaces;

	if( bAffiche )
		cout << "\nTABLEAU D'INDEX DE TEXTURES :\n";

	for( unsigned int i=0 ; i<num ; i++)
	{
		resul = false;
		while( !resul )	// Recherche du "*MESH_FACE" pour début de ligne de tableau
		{
			if( !(from>>mot) )	// Si on atteint la fin ou qu'il y a un problème avec le fichier
			{
				cerr << "\nErreur (fichier::LitTexFaces) : Fichier ASE corrompu";
				return false;	// La lecture n'a pas abouti
			}

			if( mot==mot_MESH_TFACE )
				resul = true;
		}

		from >> index >> tface1 >> tface2 >> tface3;
		tabTexFaces[3*i] = tface1;
		tabTexFaces[(3*i)+1] = tface2;
		tabTexFaces[(3*i)+2] = tface3;
		if( bAffiche )
		{
			cout<<"\n\t"<<index<<' '<<tabTexFaces[3*i]<<' '<<tabTexFaces[3*i+1]<<' '<<tabTexFaces[3*i+2];
		}
	}
	
	return true;	// Tout s'est bien passé
}

CGeoObject* geomobject( ifstream &from, CMap *pMap, unsigned int nbr ) //Lit un objet géométrique dans le
{							// fichier et le place en mémoire puis retourne son pointeur
	unsigned int accolade = 0;
	char mot_accoladeO[] = "{";
	char mot_accoladeF[] = "}";
	char mot_MESH_NUMVERTEX[] = "*MESH_NUMVERTEX";
	char mot_MESH_NUMFACES[] = "*MESH_NUMFACES";
	char mot_WIREFRAME_COLOR[] = "*WIREFRAME_COLOR";
	char mot_MESH_NUMTVERTEX[] = "*MESH_NUMTVERTEX";
	char mot_MESH_NUMTVFACES[] = "*MESH_NUMTVFACES";
	char mot_MATERIAL_REF[] = "*MATERIAL_REF";
	char mot_MESH_NORMALS[] = "*MESH_NORMALS";
	unsigned int numVertex=0, numFaces=0;

	string mot;
	
	if( bAffiche )
		cout << "\f"; //Changement de page
	
	if( !(from >> mot) )
		cerr << "\nErreur (fichier::geomobject) : non prise en compte";
	if( mot==mot_accoladeO )
		accolade = 1;
	else 
	{
		cerr << "\nErreur de lecture du fichier : pas d'accolade ouvrante après '*GEOMOBJECT' !\n";
		cerr << "\nIl n'y a plus de pertinence !";
	}
		
	while( accolade ) // Recherche le nombre de sommets apres le mot "*MESH_NUMVERTEX"
	{		
		from >> mot;
		if( mot==mot_MESH_NUMVERTEX )
		{
			from >> numVertex;

			if( numVertex!=0 )
			{
				if( bAffiche )
					cout << "\nIl y a " << numVertex << "sommets dans l'objet geometrique " << nbr;
				break;
			}
			else
			{
				cerr << "\nNombre de sommets non valide dans l'objet geometrique " << nbr;
				return NULL;
			}
		}
		else if( mot==mot_accoladeO )	// Compte une accolade
			accolade++;
		else if( mot==mot_accoladeF )	// Décompte une accolade
			accolade--;
	}

	while( accolade!=0 )	//Recherche le nombre de faces apres le mot "*MESH_NUMFACES"
	{
		from >> mot;
		if( mot==mot_MESH_NUMFACES )
		{
			from >> numFaces;
			if( numVertex!=0 )
			{
				if( bAffiche )
					cout << "\nIl y a " << numFaces <<" faces dans l'objet geometrique " << nbr;
				break;
			}
			else
			{
				cerr << "\nNombre de faces non valide\n";
				return NULL;
			}
		}
		else if( mot==mot_accoladeO )	// Compte une accolade
			accolade++;
		else if( mot==mot_accoladeF )	// Décompte une accolade
			accolade--;
	}

	CGeoObject *pGeoObject = new CGeoObject( pMap, numVertex, numFaces );

	if( bAffiche )
		cout << "\nLe fichier contient " << pGeoObject->m_NumVertex << " sommets et " << pGeoObject->m_NumFaces << "faces";
	
	if( !LitVertex( from, pGeoObject ) )						//Place les sommets en mémoire
	{
		cerr << "\nErreur non prise en charge, l'execution est obsolete";
		return NULL;
	}

	if( !LitFaces( from, numFaces, pGeoObject->m_TabFaces ) )	//Place les index en mémoire
	{
		cerr << "\nErreur non prise en charge, l'execution est obsolete";
		return NULL;
	}
	accolade++;		//LitVertex et LitFaces ne comptent pas les accolades

	while( accolade!=0 ) //Recherche la couleur de l'objet apres le mot "*WIREFRAME_COLOR"
	{
		from >> mot;
		if( mot==mot_WIREFRAME_COLOR )//Recherche l'éventuelle couleur de 'lobjet géo
		{
			float r,g,b;
			from >> r >> g >> b;
			
			pGeoObject->Color( r, g, b );

			if( bAffiche )
				cout << "\nLa couleur est " <<' '<<r<<' '<<g<<' '<<b<<  " dans l'objet géométrique " << nbr;

		}
		else if( mot==mot_MESH_NUMTVERTEX )		// Recherche les éventuels sommets de texture
		{										
			from >> pGeoObject->m_NumTexVertex;
			if( pGeoObject->m_NumTexVertex!=0 )
			{
				if( !LitTexVertex( from, pGeoObject ) )	// Lecture des sommets de texture
				{
					cerr << "\nErreur non prise en charge, l'execution est obsolete";
					return NULL;
				}

				if( bAffiche )
					cout << "\nIl y a " << pGeoObject->m_NumTexVertex << " coordonnées de texture dans l'objet géométrique " << nbr;
				accolade++; //LitTexVertex ne compte pas les accolades
			}
		}
		else if( mot==mot_MESH_NUMTVFACES )		//Recherche les éventuels index de sommets de texture
		{										
			from >> pGeoObject->m_NumTexFaces;
			if( pGeoObject->m_NumTexFaces!=0 )
			{
				if( !LitTexFaces( from, pGeoObject ) )	// Lecture des index de sommets de texture
				{
					cerr << "\nErreur non prise en charge, l'execution est obsolete";
					return NULL;
				}

				if( bAffiche )
					cout << "\nIl y a " << pGeoObject->m_NumTexFaces << " index de texture dans l'objet géométrique " << nbr;
				accolade++;		//LitTexFaces ne compte pas les accolades
			}
		}
		if( mot==mot_MESH_NORMALS )		//Recherche les vecteurs normaux
		{
			if( !LitNormaux( from, pGeoObject ) )	// Lecture des vetceurs normaux
			{
				cerr << "\nErreur non prise en charge, l'execution est obsolete";
				return NULL;
			}
			accolade++; 
		}
		if( mot==mot_MATERIAL_REF )		//Recherche l'éventuelle référence à un matériau
		{
			unsigned int matRef;
			from >> matRef;
			pGeoObject->CreateMaterial( matRef );	// Associe l'objet au matériau
			if( bAffiche )
				cout << "\nReference au matériau " << matRef << " dans l'objet geometrique " << nbr;
		}
		else if( mot==mot_accoladeO )
			accolade++;
		else if( mot==mot_accoladeF )
			accolade--;
	}	

	return pGeoObject;
}

CGeoMaterial *materiallist( ifstream &from )
{
	unsigned int accolade2 = 0;
	char mot_accoladeO[] = "{";
	char mot_accoladeF[] = "}";
	char mot_MATERIAL[] = "*MATERIAL";
	char mot_MATERIAL_AMBIENT[] = "*MATERIAL_AMBIENT";
	char mot_MATERIAL_DIFFUSE[] = "*MATERIAL_DIFFUSE";
	char mot_MATERIAL_SPECULAR[] = "*MATERIAL_SPECULAR";
	char mot_BITMAP[] = "*BITMAP";

	string mot;

	CGeoMaterial *pGeoMaterial = new CGeoMaterial;
	if( bAffiche )
		cout << "\nLISTE DE MATOS";

	from >> mot;
	while( mot!=mot_MATERIAL )		//Recherche l'éventuelle couleur de l'objet géo
	{
		if( !(from >> mot) )
		{
			cerr << "\nErreur (fichier) : Fichier ASE corrompu";
			return NULL;
		}
	}
			
	from >> pGeoMaterial->m_Ref;	
	if( bAffiche )
		cout << "\nLa reference est : " << pGeoMaterial->m_Ref;

	do
	{
		from >> mot;

		if( mot==mot_MATERIAL_AMBIENT )			//Recherche l'éventuelle couleur ambiante de l'objet geo
		{
			from >> pGeoMaterial->m_MaterialAmbient[0] >> pGeoMaterial->m_MaterialAmbient[1] >> pGeoMaterial->m_MaterialAmbient[2];
			if(bAffiche)
			{
				cout << "\nCouleur ambiante : ";
				cout<<pGeoMaterial->m_MaterialAmbient[0]<<' '<<pGeoMaterial->m_MaterialAmbient[1]<<' '<<pGeoMaterial->m_MaterialAmbient[2];
			}
		}
		else if( mot==mot_MATERIAL_DIFFUSE )	//Recherche l'éventuelle couleur diffuse de l'objet géo
		{
			from >> pGeoMaterial->m_MaterialDiffuse[0] >> pGeoMaterial->m_MaterialDiffuse[1] >> pGeoMaterial->m_MaterialDiffuse[2];
			if(bAffiche)
			{
				cout << "\nCouleur diffuse : ";
				cout<<pGeoMaterial->m_MaterialDiffuse[0]<<' '<<pGeoMaterial->m_MaterialDiffuse[1]<<' '<<pGeoMaterial->m_MaterialDiffuse[2];
			}
		}
		else if( mot==mot_MATERIAL_SPECULAR )	//Recherche l'éventuelle couleur spéculaire de l'objet géo
		{
			from >> pGeoMaterial->m_MaterialSpecular[0] >> pGeoMaterial->m_MaterialSpecular[1] >> pGeoMaterial->m_MaterialSpecular[2];	
			if( bAffiche ) 
			{
				cout << "\nCouleur specular : ";
				cout<<pGeoMaterial->m_MaterialSpecular[0]<<' '<<pGeoMaterial->m_MaterialSpecular[1]<<' '<<pGeoMaterial->m_MaterialSpecular[2];
			}
		}
		else if( mot==mot_BITMAP )	// Lecture du nom de fichier de texture
		{
			string nomFichier;
			from >> nomFichier;
				
				// Enlève le premier guillemet
			int coucou = nomFichier.find_first_of( '"' );
			if( coucou!=npos )
				nomFichier.erase( coucou, 1 );
				// Enlève le dernier guillemet
			coucou = nomFichier.find_last_of( '"' );
			if( coucou!=npos )
				nomFichier.erase( coucou, 1 );

			pGeoMaterial->m_FichierTexture = nomFichier;

			if( bAffiche )
				cout << "\nNom du fichier de texture : " << pGeoMaterial->m_FichierTexture;
		}
		else if( mot==mot_accoladeO )
			accolade2++;
		else if( mot==mot_accoladeF )
			accolade2--;
	}
	while( accolade2!=0 );
			
	return pGeoMaterial;
}
	
CLight *lightList( ifstream &from )
{
	unsigned int accolade = 0;
	char mot_accoladeO[] = "{";
	char mot_accoladeF[] = "}";
	char mot_LIGHT_COLOR[] = "*LIGHT_COLOR";
	char mot_TM_POS[] = "*TM_POS"; 

	string mot;

	CLight *pLight = new CLight;
	if( bAffiche )
		cout << "\nLISTE DE LUMIERE(S)";
	
	float fPos1, fPos2, fPos3;

	do
	{
		from >> mot;
		
		if( mot==mot_LIGHT_COLOR )	// Recherche l'éventuelle couleur de 'lobjet géo
		{
			from >> pLight->m_Color[0] >> pLight->m_Color[1] >> pLight->m_Color[2];	
			if( bAffiche )
			{
				cout << "\nCouleur de la lumière : ";
				cout<<pLight->m_Color[0]<<' '<<pLight->m_Color[1]<<' '<<pLight->m_Color[2];
			}
		}
		else if( mot==mot_TM_POS )	//Recherche la position de la lumière
		{
			from >> fPos1 >> fPos2 >> fPos3;	
			pLight->m_Position[0] = +fPos3/fScale*100/62;
			pLight->m_Position[1] = +fPos3/fScale*80/62;
			pLight->m_Position[2] = 0;//-fPos3*80/62/fScale;
			if( bAffiche )
			{
				cout << "\nPosition de la lumière : ";
				cout<<pLight->m_Position[0]<<' '<<pLight->m_Position[1]<<' '<<pLight->m_Position[2];
			}
		}
		else if( mot==mot_accoladeO )
				accolade++;
		else if( mot==mot_accoladeF )
				accolade--;
	}
	while( accolade!=0 );

	return pLight;
}

bool LitFichier( const string &nomFichier, CMap *pMap )
{
	cout.fill( ' ' );
	string nomFichierASE = ".\\ase\\" + nomFichier + ".ase";	// Ajout de l'extension et du chemin
	
		// Ouverture du fichier ASE
	ifstream from( nomFichierASE.c_str() );
	if( !from )
	{
		cerr << endl << "Erreur (fichier) : Echec d'ouverture du fichier " << nomFichierASE;
		return false;	// La lecture n'a pas abouti
	}

	pMap->m_pTabGeo = new Tableau<CGeoObject>;			//crée la liste d'objets géométriques
	pMap->m_pTabMaterial = new Tableau<CGeoMaterial>;	//crée la liste des matériaux
	pMap->m_pTabLight = new Tableau<CLight>;			//crée la liste des lumières

	unsigned int nbr_GeoObject = 0;

	char mot_GEOMOBJECT[] = "*GEOMOBJECT";
	char mot_MATERIAL_LIST[] = "*MATERIAL_LIST";
	char mot_LIGHTOBJECT[] ="*LIGHTOBJECT";


	bAffiche = Config.Debug.bAfficheFichier; 

	if( bAffiche )
		cout << "Nom du fichier ASE : " << nomFichierASE << endl;

	string mot;		// Variable muette
	
	while( from >> mot )
	{
		switch( mot[0] )
		{
		case '*' :
			if( mot==mot_GEOMOBJECT )		// Recherche des objets géométriques
			{	
				nbr_GeoObject++;
				CGeoObject *pGeo;

				pGeo = geomobject( from, pMap, nbr_GeoObject );
				if( pGeo )	// Si pas d'erreur mets le dans le tableau d'objets géo
				{
					pMap->m_pTabGeo->Ajoute( pGeo );
				}
				else
				{
					cerr << "\nERREUR 1 geo\t";
					return false;					// La lecture n'a pas abouti
				}
				
				break;
			}

			if( mot==mot_LIGHTOBJECT )			// Recherche des limières
			{	
				CLight *pLight;

				pLight = lightList( from );
				if( pLight )		//Si pas d'erreur mets le dans le tableau d'objets géo
					pMap->m_pTabLight->Ajoute( pLight );
				else				//Sinon libère sa mémoire
				{
					cerr << "\nERREUR 1 light\t";
					return false;					// La lecture n'a pas abouti
				}

				break;
			}

			if( mot==mot_MATERIAL_LIST )		// Recherche des matériaux
			{	
				char mot_MATERIAL_COUNT[] = "*MATERIAL_COUNT";
				CGeoMaterial *pGeoMaterial;
				int nbr_materiau = -1;	// Nombre de matériaux du fichier ASE
				
				while( (nbr_materiau==-1) && (from>>mot) )
				{
					if( mot==mot_MATERIAL_COUNT )		//Recherche du nombre de matériaux
					{
						from >> nbr_materiau;		// Récupère le nombre de matériaux
						if( bAffiche )
							cout << "Nombre de matériaux : " << nbr_materiau << endl;
					}
				}

				while( nbr_materiau!=0 )
				{
					pGeoMaterial = materiallist( from );
					if( pGeoMaterial )
					{
						pMap->m_pTabMaterial->Ajoute( pGeoMaterial ) ;
					}
					else
					{
						cerr << "\nERREUR 1 materiau\t";
						return false;					// La lecture n'a pas abouti
					}
					if( bAffiche )
						cout << "\nMateriau " << nbr_materiau << "repertorie\n";
					nbr_materiau--;
				}

				break;
			}

		}
	}

	return true;
}
