/*
 * AdvancedConfigurationDto.h
 *
 *  Created on: 16 mai 2012
 *      Author: Erwin
 */

#ifndef ADVANCEDCONFIGURATIONDTO_H_
#define ADVANCEDCONFIGURATIONDTO_H_

namespace jkt {

class AdvancedConfigurationDto {
	bool _isPlayeurOutlineVisible;
	bool _isPlayerSkinVisible;

	float _playerCameraHeight;

public:
	AdvancedConfigurationDto();
	virtual ~AdvancedConfigurationDto();

	bool isPlayerSkinVisible() const;
	void setIsPlayerSkinVisible(const bool isPlayerSkinVisible);

	bool isPlayeurOutlineVisible() const;
	void setIsPlayeurOutlineVisible(const bool isPlayeurOutlineVisible);

	float getPlayerCameraHeight() const;
	void setPlayerCameraHeight(const float playerCameraHeight);
};

} /* namespace jkt */
#endif /* ADVANCEDCONFIGURATIONDTO_H_ */
