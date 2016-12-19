/*
 * PlayersWebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */


#include <stdlib.h>

#include "util/Trace.h"
#include "reseau/web/HttpServer.h"
#include "reseau/web/json/JsonObject.h"
#include "service/PlayersService.h"
#include "service/dto/PlayerInformationDto.h"

#include "reseau/web/service/PlayersWebService.h"

namespace jkt {

PlayersWebService::PlayersWebService() {
}

PlayersWebService::~PlayersWebService() {
}

WebServiceResult PlayersWebService::getPlayersInCurrentMap() {
	JsonObject root;
	JsonList& players = root.addList("players");

	vector<PlayerInformationDto> dtos;
	PlayersService::getPlayers(dtos);

	for(PlayerInformationDto dto : dtos) {
		JsonObject& jsonPlayer = players.addObject();
		jsonPlayer.addString("name", dto.getName());
	}

	return WebServiceResult(root, HttpServer::HTTP_RESPONSE_200);
}

WebServiceResult PlayersWebService::execute(HttpServer::HTTP_METHODS method, const string& fullEndpoint, const string& baseEndpoint, const string& serviceEndpoint, const std::string& params) {
	if(method == HttpServer::HTTP_METHODS::HTTP_GET && serviceEndpoint == "players") {
		return getPlayersInCurrentMap();
	}

	// Service not found
	throw HttpServer::HTTP_EXCEPTION::SERVICE_NOT_EXISTS;
}

} /* namespace jkt */

