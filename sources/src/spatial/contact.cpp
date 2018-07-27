
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <fstream>
#include <map>

#include "util/V3D.h"
#include "util/math_vectoriel.h"
#include "main/Player.h"

#include "spatial/contact.h"

using namespace jkt;
using namespace std;

namespace jkt {

void contactPlayer(CPlayer *player, float *normal, float distanceW) {
	float vitesse[3];
	player->getVitesse( vitesse );
	float var = -(vitesse[0]*normal[0]) - (vitesse[1]*normal[1]) + (vitesse[2]*normal[2]);

	if( distanceW > 0.0 ) {		// Vérifie si le joueur s'approche ou s'éloigne de la surface du triangle
		if( var > 0.0) {
			vitesse[0] += var*normal[0]; 	// Rend le vecteur vitesse du joueur parallèle à la surface du triangle
			vitesse[1] += var*normal[1];
			vitesse[2] -= var*normal[2];
		}
	}
	else {						// Vérifie si le joueur s'approche ou s'éloigne de la surface du triangle
		if( var < 0.0) {
			vitesse[0] += var*normal[0]; 	// Rend le vecteur vitesse du joueur parallèle à la surface du triangle
			vitesse[1] += var*normal[1];
			vitesse[2] -= var*normal[2];
		}
	}

	player->setVitesse( vitesse );

	// Parmi les objets en contact avec le joeur, cherche celui dont la pente est la plus faible
	// Un sol horizontal sous les pieds du joueur a une pente de 0, un mur vertical contre le joueur a une pente de 1
	float pente = 1.0f - fabsf(normal[1]);

	if( pente < player->pente() ) {			// Cherche la pente la plus horizontale subie par le joueur
		player->pente( pente );				// Donne la pente du triangle en contact / rapport à l'horizontale
	}
}

void contactSprite(CPlayer *sprite, float *normal, float distanceW) {	// fonction de rebond sur les parois
	float vitesse[3];
	sprite->getVitesse( vitesse );
	float var = -(vitesse[0]*normal[0]) - (vitesse[1]*normal[1]) + (vitesse[2]*normal[2]);

	if( distanceW > 0.0 ) {	// Vérifie si le joueur s'approche ou s'éloigne de la surface du triangle
		if( var > 0.0) {
			float oldNorm = norme(vitesse);
			vitesse[0] += 2*var*normal[0];	//inverse la composante du vecteur vitesse
			vitesse[1] += 2*var*normal[1];	//orthogonale � la surface du triangle, pour
			vitesse[2] -= 2*var*normal[2];	//produire un effet de rebond
			normalise(vitesse);				//tout en conservant la norme
			vitesse[0] *= oldNorm;
			vitesse[1] *= oldNorm;
			vitesse[2] *= oldNorm;
		}
	}
	else {					//v�rifie si le joueur s'approche ou s'�loigne de la surface du triangle
		if( var < 0.0) {
			float oldNorm = norme(vitesse);
			vitesse[0] += 2*var*normal[0];	//inverse la composante du vecteur vitesse
			vitesse[1] += 2*var*normal[1];	//orthogonale � la surface du triangle, pour
			vitesse[2] -= 2*var*normal[2];	//produire un effet de rebond
			normalise(vitesse);				//tout en conservant la norme
			vitesse[0] *= oldNorm;
			vitesse[1] *= oldNorm;
			vitesse[2] *= oldNorm;
		}
	}

	sprite->setVitesse( vitesse );
}

}	// JktMoteur
