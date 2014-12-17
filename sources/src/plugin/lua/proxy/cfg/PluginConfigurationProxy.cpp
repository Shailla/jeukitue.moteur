/*
 * PluginConfigurationProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>
#include <sstream>

using namespace std;

#include "fmod.h"

#include "main/Fabrique.h"
#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"
#include "main/Cfg.h"
#include "service/InformationService.h"
#include "service/dto/VersionsDto.h"

#include "plugin/lua/proxy/cfg/PluginConfigurationProxy.h"

extern CCfg Config;		// Contient la configuration du jeu

namespace JktPlugin {

const char* PluginConfigurationProxy::CFG_PARAM_AUDIO_DRIVER = "AUDIO_DRIVER";								// Paramètre de type nombre
const char* PluginConfigurationProxy::CFG_PARAM_AUDIO_RECORD_DRIVER = "AUDIO_RECORD_DRIVER";				// Paramètre de type nombre
const char* PluginConfigurationProxy::CFG_PARAM_AUDIO_MIXER = "AUDIO_MIXER";								// Paramètre de type nombre
const char* PluginConfigurationProxy::CFG_PARAM_OUTPUT = "AUDIO_OUTPUT";									// Paramètre de type nombre

const char* PluginConfigurationProxy::CFG_PARAM_SKIN_VISIBILITY = "SKIN_VISIBILITY";						// Paramètre de type booléen
const char* PluginConfigurationProxy::CFG_PARAM_PLAYER_OUTLINE_VISIBILITY = "PLAYER_OUTLINE_VISIBILITY";	// Paramètre de type booléen
const char* PluginConfigurationProxy::CFG_PARAM_CUBIC_METER_VISIBILITY = "CUBIC_METER_VISIBILITY";			// Paramètre de type booléen
const char* PluginConfigurationProxy::CFG_PARAM_AXES_METER_VISIBILITY = "AXES_METER_VISIBILITY";			// Paramètre de type booléen

const char* PluginConfigurationProxy::CST_COMPILATION_DATE = "COMPILATION_DATE";							// Paramètre de type chaîne de caractères
const char* PluginConfigurationProxy::CST_OPENGL_VERSION = "OPENGL_VERSION";								// Paramètre de type chaîne de caractères
const char* PluginConfigurationProxy::CST_GLU_VERSION = "GLU_VERSION";										// Paramètre de type chaîne de caractères
const char* PluginConfigurationProxy::CST_FMOD_VERSION = "FMOD_VERSION";									// Paramètre de type chaîne de caractères
const char* PluginConfigurationProxy::CST_AGAR_VERSION = "AGAR_VERSION";									// Paramètre de type chaîne de caractères

/**
 * Obtenir les dimensions de l'écran.
 *    - Return 1 : Dimension horizontale en pixels
 *    - Return 2 : Dimension verticale en pixels
 */
int PluginConfigurationProxy::getScreenSize(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();

	lua_pushnumber(L, videoInfo->current_w);
	lua_pushnumber(L, videoInfo->current_h);

	return 2;
}

/**
 * Obtenir la liste des drivers audios disponibles.
 *    - Return 1 : Nombre identifiant du driver
 *    - Return 2 : Nom du driver
 */
int PluginConfigurationProxy::getAvailableAudioDrivers(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	int nbrAudioDrivers = FSOUND_GetNumDrivers();

	lua_newtable(L);

	for(int i=0 ; i<nbrAudioDrivers ; i++) {
		lua_pushinteger(L, i);
		lua_pushstring(L, FSOUND_GetDriverName(i));
		lua_settable(L, -3);
	}

	return 1;
}

/**
 * Obtenir la liste des drivers audios disponibles.
 *    - Return 1 : Nombre identifiant du driver
 *    - Return 2 : Nom du driver
 */
int PluginConfigurationProxy::getAvailableAudioRecordDrivers(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	int nbrAudioDrivers = FSOUND_Record_GetNumDrivers();

	lua_newtable(L);

	for(int i=0 ; i<nbrAudioDrivers ; i++) {
		lua_pushinteger(L, i);
		lua_pushstring(L, FSOUND_Record_GetDriverName(i));
		lua_settable(L, -3);
	}

	return 1;
}

/**
 * Obtenir la liste des sorties son disponibles.
 *    - Return 1 : Nombre identifiant la sortie
 *    - Return 2 : Nom de la sortie
 */
int PluginConfigurationProxy::getAvailableAudioOutputs(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	lua_newtable(L);

	for(int i=0 ; i < CCfg::CAudio::AVAILABLE_AUDIO_OUTPUTS_NBR ; i++) {
		lua_pushinteger(L, i);
		lua_pushstring(L, CCfg::CAudio::AVAILABLE_AUDIO_OUTPUTS[i]);
		lua_settable(L, -3);
	}

	return 1;
}

/**
 * Enregistre la configuration courante du jeu.
 */
int PluginConfigurationProxy::saveConfiguration(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		Config.Ecrit();
	}

	return 0;
}

/**
 * Retourne la valeur d'un paramètre global de configuration du moteur.
 *    - Param 1 : nom du paramètre demandé
 *    - Return 1 : valeur du paramètre
 */
int PluginConfigurationProxy::getConfigurationParameter(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		const string paramName = lua_tostring(L, 1);

		if(paramName.compare(CFG_PARAM_AUDIO_DRIVER) == 0) {
			lua_pushnumber(L, Config.Audio.m_Driver);
			return 1;
		}
		else if(paramName.compare(CFG_PARAM_AUDIO_RECORD_DRIVER) == 0) {
			lua_pushnumber(L, Config.Audio.m_DriverRecord);
			return 1;
		}
		else if(paramName.compare(CFG_PARAM_AUDIO_MIXER) == 0) {
			lua_pushnumber(L, Config.Audio.m_Mixer);
			return 1;
		}
		else if(paramName.compare(CFG_PARAM_OUTPUT) == 0) {
			lua_pushnumber(L, Config.Audio.m_Output);
			return 1;
		}
		else if(paramName.compare(CFG_PARAM_SKIN_VISIBILITY) == 0) {
			lua_pushboolean(L, Config.Joueur.skinVisibility);
			return 1;
		}
		else if(paramName.compare(CFG_PARAM_PLAYER_OUTLINE_VISIBILITY) == 0) {
			lua_pushboolean(L, Config.Joueur.outlineVisibility);
			return 1;
		}
		else if(paramName.compare(CFG_PARAM_CUBIC_METER_VISIBILITY) == 0) {
			lua_pushboolean(L, Config.Debug.cubicMeterVisibility);
			return 1;
		}
		else if(paramName.compare(CFG_PARAM_AXES_METER_VISIBILITY) == 0) {
			lua_pushboolean(L, Config.Debug.axesMeterVisibility);
			return 1;
		}
		else {
			ostringstream message;
			message << "Nom du parametre de configuration inconnu (recu='" << paramName << "')";
			Fabrique::getPluginEngine()->getGlobalPluginContext(L)->logScriptError(message.str());
			return 0;
		}
	}

	return 1;
}

/**
 * Retourne la valeur d'une constante du moteur.
 *    - Param 1 : nom de la constante demandée
 *    - Return 1 : valeur de la constante
 */
int PluginConfigurationProxy::getConstant(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		const string cstName = lua_tostring(L, 1);

		if(cstName.compare(CST_COMPILATION_DATE) == 0) {
			lua_pushstring(L, __DATE__);
			return 1;
		}
		else if(cstName.compare(CST_OPENGL_VERSION) == 0) {
			JktService::VersionsDto versions = JktService::InformationService::loadVersions();
			lua_pushstring(L, versions.getOpenGlVersion().c_str());
			return 1;
		}
		else if(cstName.compare(CST_GLU_VERSION) == 0) {
			JktService::VersionsDto versions = JktService::InformationService::loadVersions();
			lua_pushstring(L, versions.getGluVersion().c_str());
			return 1;
		}
		else if(cstName.compare(CST_FMOD_VERSION) == 0) {
			JktService::VersionsDto versions = JktService::InformationService::loadVersions();
			lua_pushstring(L, versions.getFmodVersion().c_str());
			return 1;
		}
		else if(cstName.compare(CST_AGAR_VERSION) == 0) {
			JktService::VersionsDto versions = JktService::InformationService::loadVersions();
			lua_pushstring(L, versions.getAgarVersion().c_str());
			return 1;
		}
		else {
			ostringstream message;
			message << "Nom de constante inconnu (recu='" << cstName << "')";
			Fabrique::getPluginEngine()->getGlobalPluginContext(L)->logScriptError(message.str());
			return 0;
		}
	}

	return 1;
}

/**
 * Change la valeur d'un paramètre global de configuration du moteur.
 *    - Param 1 : nom du paramètre demandé
 *    - Param 2 : nouvelle valeur du paramètre
 */
int PluginConfigurationProxy::setConfigurationParameter(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 2, LUA_PARAM_STRING, LUA_PARAM_ANY)) {
		const string paramName = lua_tostring(L, 1);

		if(paramName.compare(CFG_PARAM_AUDIO_DRIVER) == 0) {
			int value = lua_tonumber(L, 2);
			Config.Audio.m_Driver = value;
		}
		else if(paramName.compare(CFG_PARAM_AUDIO_RECORD_DRIVER) == 0) {
			int value = lua_tonumber(L, 2);
			Config.Audio.m_DriverRecord = value;
		}
		else if(paramName.compare(CFG_PARAM_AUDIO_MIXER) == 0) {
			int value = lua_tonumber(L, 2);
			Config.Audio.m_Mixer = value;
		}
		else if(paramName.compare(CFG_PARAM_OUTPUT) == 0) {
			int value = lua_tonumber(L, 2);
			Config.Audio.m_Output = value;
		}
		else if(paramName.compare(CFG_PARAM_SKIN_VISIBILITY) == 0) {
			bool value = lua_toboolean(L, 2);
			Config.Joueur.skinVisibility = value;
		}
		else if(paramName.compare(CFG_PARAM_PLAYER_OUTLINE_VISIBILITY) == 0) {
			bool value = lua_toboolean(L, 2);
			Config.Joueur.outlineVisibility = value;
		}
		else if(paramName.compare(CFG_PARAM_CUBIC_METER_VISIBILITY) == 0) {
			bool value = lua_toboolean(L, 2);
			Config.Debug.cubicMeterVisibility = value;
		}
		else if(paramName.compare(CFG_PARAM_AXES_METER_VISIBILITY) == 0) {
			bool value = lua_toboolean(L, 2);
			Config.Debug.axesMeterVisibility = value;
		}
		else {
			ostringstream message;
			message << "Nom du parametre de configuration inconnu (recu='" << paramName << "')";
			Fabrique::getPluginEngine()->getGlobalPluginContext(L)->logScriptError(message.str());
		}
	}

	return 0;
}

/**
 * Initialise les drivers audio.
 *    - Param 1 : identifiant du driver audio principal à activer
 *    - Param 2 : identifiant de la sortie audio à activer
 *    - Param 3 : identifiant du mixer à activer
 *    - Param 4 : identifiant du driver d'enregistrement à activer
 *    - Return : 'true' en cas de succès, 'false' sinon
 */
int PluginConfigurationProxy::initAudio(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 4, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER)) {
		const int audioDriver = lua_tonumber(L, 1);
		const int audioOutput = lua_tonumber(L, 1);
		const int audioMixer = lua_tonumber(L, 1);
		const int audioRecordDriver = lua_tonumber(L, 1);

		bool result = Config.Audio.testInitAndSaveConfiguration(audioDriver, audioOutput, audioMixer, audioRecordDriver);
		lua_pushboolean(L, result);

		return 1;
	}

	return 0;
}

} /* namespace JktPlugin */
