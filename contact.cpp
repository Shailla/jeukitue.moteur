
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <fstream>
#include <map>

#include "V3D.h"
#include "math_vectoriel.h"
#include "Player.h"

#include "contact.h"

using namespace JKT_PACKAGE_UTILS;
using namespace std;

namespace JKT_PACKAGE_MOTEUR3D
{

void contactPlayer(CPlayer *player, float *normal, float distanceW)
{
	float vitesse[3];
	player->getVitesse( vitesse );
	float caca = -(vitesse[0]*normal[0])
			-(vitesse[1]*normal[1])
			+(vitesse[2]*normal[2]);

	if( distanceW > 0.0 )	//vérifie si le joueur
	{						//s'approche ou s'éloigne
		if( caca > 0.0)		//de la surface du triangle
		{				
			vitesse[0] += caca*normal[0]; //rend le vecteur vitesse du joueur parallèle
			vitesse[1] += caca*normal[1];	//à la surface du triangle
			vitesse[2] -= caca*normal[2]; 
		}
	}
	else					//vérifie si le joueur
	{						//s'approche ou s'éloigne
		if( caca < 0.0)		//de la surface du triangle
		{					
			vitesse[0] += caca*normal[0]; //rend le vecteur vitesse du joueur parallèle
			vitesse[1] += caca*normal[1];	//à la surface du triangle
			vitesse[2] -= caca*normal[2];
		}
	}

	player->setVitesse( vitesse );

	if( (fabsf(normal[1]))>player->Pente() )	//Cherche la pente la plus horizontale subie par le joueur
	{
		player->Pente( normal[1] );	//Donne la pente du triangle en contact /rapport à l'horizontale
	}
}

void contactSprite(CPlayer *sprite, float *normal, float distanceW)	//fonction de rebond sur
{																	//les parois
	float vitesse[3];
	sprite->getVitesse( vitesse );
	float caca = -(vitesse[0]*normal[0])
			-(vitesse[1]*normal[1])
			+(vitesse[2]*normal[2]);

	if( distanceW > 0.0 )	//vérifie si le joueur
	{						//s'approche ou s'éloigne
		if( caca > 0.0)		//de la surface du triangle
		{				
			float oldNorm = norme(vitesse);
			vitesse[0] += 2*caca*normal[0];	//inverse la composante du vecteur vitesse
			vitesse[1] += 2*caca*normal[1];	//orthogonale à la surface du triangle, pour
			vitesse[2] -= 2*caca*normal[2];	//produire un effet de rebond
			normalise(vitesse);				//tout en conservant la norme
			vitesse[0] *= oldNorm; 
			vitesse[1] *= oldNorm;
			vitesse[2] *= oldNorm;
		}
	}
	else					//vérifie si le joueur
	{						//s'approche ou s'éloigne
		if( caca < 0.0)		//de la surface du triangle
		{
			float oldNorm = norme(vitesse);
			vitesse[0] += 2*caca*normal[0];	//inverse la composante du vecteur vitesse
			vitesse[1] += 2*caca*normal[1];	//orthogonale à la surface du triangle, pour
			vitesse[2] -= 2*caca*normal[2];	//produire un effet de rebond
			normalise(vitesse);				//tout en conservant la norme
			vitesse[0] *= oldNorm; 
			vitesse[1] *= oldNorm;
			vitesse[2] *= oldNorm;						
		}
	}

	sprite->setVitesse( vitesse );
}

}	// JKT_PACKAGE_MOTEUR3D
