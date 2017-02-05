/*
 * Drawable.cpp
 *
 *  Created on: 29 juil. 2016
 *      Author: VGDJ7997
 */

#include "spatial/basic/Drawable.h"

namespace jkt {

Drawable::Drawable() {
	_highlighted = false;
	_hidden = false;
}

Drawable::Drawable(const Drawable& other) {
	_hidden = other._hidden;
	_highlighted = other._highlighted;
}

Drawable::~Drawable() {
}

bool Drawable::isHidden() const {
	return _hidden;
}

void Drawable::hide(bool hidden) {
	_hidden = hidden;
}

bool Drawable::isHighlighted() const {
	return _highlighted;
}

void Drawable::highlight(bool highlighted) {
	_highlighted = highlighted;
}

} /* namespace jkt */
