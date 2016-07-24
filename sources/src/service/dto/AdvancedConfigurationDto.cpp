/*
 * AdvancedConfigurationDto.cpp
 *
 *  Created on: 16 mai 2012
 *      Author: Erwin
 */

#include "service/dto/AdvancedConfigurationDto.h"

namespace jkt {

AdvancedConfigurationDto::AdvancedConfigurationDto() {
	_playerCameraHeight = 0.0f;
	_isPlayeurOutlineVisible = false;
	_isPlayerSkinVisible = false;
}

AdvancedConfigurationDto::~AdvancedConfigurationDto() {
}

bool AdvancedConfigurationDto::isPlayerSkinVisible() const {
	return _isPlayerSkinVisible;
}

void AdvancedConfigurationDto::setIsPlayerSkinVisible(const bool isPlayerSkinVisible) {
	_isPlayerSkinVisible = isPlayerSkinVisible;
}

bool AdvancedConfigurationDto::isPlayeurOutlineVisible() const {
	return _isPlayeurOutlineVisible;
}

void AdvancedConfigurationDto::setIsPlayeurOutlineVisible(const bool isPlayeurOutlineVisible) {
	_isPlayeurOutlineVisible = isPlayeurOutlineVisible;
}

float AdvancedConfigurationDto::getPlayerCameraHeight() const {
	return _playerCameraHeight;
}

void AdvancedConfigurationDto::setPlayerCameraHeight(const float playerCameraHeight) {
	_playerCameraHeight = playerCameraHeight;
}

} /* namespace jkt */
