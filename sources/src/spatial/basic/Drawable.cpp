/*
 * Drawable.cpp
 *
 *  Created on: 29 juil. 2016
 *      Author: VGDJ7997
 */

#include "spatial/basic/Drawable.h"

namespace jkt {

Drawable::Drawable() {
	_selected = false;
}

Drawable::Drawable(const Drawable& other) {
	_selected = other._selected;
}

Drawable::~Drawable() {
}

bool Drawable::isSelected() const {
	return _selected;
}

void Drawable::select(bool selected) {
	_selected = selected;
}

} /* namespace jkt */
