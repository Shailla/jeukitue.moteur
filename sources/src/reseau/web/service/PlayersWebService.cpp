/*
 * PlayersWebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#include <reseau/web/HttpServer.h>
#include <stdlib.h>

#include "util/Trace.h"
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

	string json = root.toString();
	LOGINFO(("REPONSE : '%s'", json.c_str()));

	WebServiceResult result;
	result._contentSize = json.size();
	result._content = malloc(json.size());
	json.copy((char*)result._content, json.size());
	result._contentType = HttpServer::HTTP_CONTENT_TYPE_JSON;
	result._status = HttpServer::HTTP_RESPONSE_200;

	return result;
}

WebServiceResult PlayersWebService::execute(HttpServer::HTTP_METHODS method, const string& fullEndpoint, const string& baseEndpoint, const string& serviceEndpoint, const std::string& params) {
	if(method == HttpServer::HTTP_METHODS::HTTP_GET && serviceEndpoint == "players") {
		return getPlayersInCurrentMap();
	}
	else {
		// TODO return unexisting requested service
	}
}

} /* namespace jkt */

