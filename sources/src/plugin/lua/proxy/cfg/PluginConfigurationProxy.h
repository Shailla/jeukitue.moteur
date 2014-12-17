/*
 * PluginConfigurationProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINCONFIGURATIONPROXY_H_
#define PLUGINCONFIGURATIONPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace JktPlugin {

class PluginConfigurationProxy {
	PluginConfigurationProxy() {}		// Singleton

	static const char* CFG_PARAM_AUDIO_DRIVER;
	static const char* CFG_PARAM_AUDIO_RECORD_DRIVER;
	static const char* CFG_PARAM_AUDIO_MIXER;
	static const char* CFG_PARAM_OUTPUT;

	static const char* CFG_PARAM_SKIN_VISIBILITY;
	static const char* CFG_PARAM_PLAYER_OUTLINE_VISIBILITY;
	static const char* CFG_PARAM_CUBIC_METER_VISIBILITY;
	static const char* CFG_PARAM_AXES_METER_VISIBILITY;

	static const char* CST_COMPILATION_DATE;
	static const char* CST_OPENGL_VERSION;
	static const char* CST_GLU_VERSION;
	static const char* CST_FMOD_VERSION;
	static const char* CST_AGAR_VERSION;

public:
	static int saveConfiguration(lua_State *L);

	static int getScreenSize(lua_State *L);

	/** Get the list of the available audio drivers on the computer */
	static int getAvailableAudioDrivers(lua_State *L);

	/** Get the list of the available audio record drivers on the computer */
	static int getAvailableAudioRecordDrivers(lua_State *L);

	/** Get the list of the available audio outputs on the computer */
	static int getAvailableAudioOutputs(lua_State *L);

	static int getConfigurationParameter(lua_State* L);
	static int setConfigurationParameter(lua_State* L);

	static int getConstant(lua_State* L);

	static int initAudio(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINCONFIGURATIONPROXY_H_ */
