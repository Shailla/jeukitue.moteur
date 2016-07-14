/*
 * FonteEngine.cpp
 *
 *  Created on: 11 juil. 2016
 *      Author: Erwin
 */

#include <vector>

#include "util/Trace.h"

#include "util/fonte/FonteEngine.h"

FonteEngine::FonteEngine() :_ft() {
	if(FT_Init_FreeType(&_ft)) {
		LOGERROR(("Librairie FreeType introuvable"));
	}
	else {
		LOGINFO(("Librairie FreeType initialisée"));
	}

}

FonteEngine::~FonteEngine() {
}

Fonte* FonteEngine::loadFonte(const string& fonte, int height) {
	FT_Face face;

	/* ************************************** */
	/* Charge la fonte                        */
	/* ************************************** */

	if(FT_New_Face(_ft, fonte.c_str(), 0, &face)) {
		LOGERROR(("Echec d'ouverture de la fonte : %s", fonte.c_str()));
	}
	else {
		LOGINFO(("Fonte chargée : %s", fonte.c_str()));
	}

	FT_Set_Pixel_Sizes(face, 0, height);

	return new Fonte(face);
}
