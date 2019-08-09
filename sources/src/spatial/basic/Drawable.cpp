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
	_volatileHighlighted = false;
}

Drawable::Drawable(const Drawable& other) {
	_hidden = other._hidden;
	_highlighted = other._highlighted;
	_volatileHighlighted = false;
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

bool Drawable::isVolatileHighlighted() const {
	return _volatileHighlighted;
}

void Drawable::getVolatileHighlightedColor(float& r, float& v, float& b) const {
	r = _volatileHighlightedColor[0];
	v = _volatileHighlightedColor[1];
	b = _volatileHighlightedColor[2];
}

void Drawable::highlight(bool highlighted) {
	_highlighted = highlighted;
}

void Drawable::setVolatileHighlighted(float r, float v, float b) {
	_volatileHighlighted = true;
	_volatileHighlightedColor[0] = r;
	_volatileHighlightedColor[1] = v;
	_volatileHighlightedColor[2] = b;
}

void Drawable::unsetVolatileHighlighted() {
	_volatileHighlighted = false;
}

} /* namespace jkt */
