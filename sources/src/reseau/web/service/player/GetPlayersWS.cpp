/*
 * GetPlayersWS.cpp
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
#include "reseau/web/HttpException.h"

#include "reseau/web/service/player/GetPlayersWS.h"

using namespace std;

namespace jkt {

GetPlayersWS::GetPlayersWS() {
}

GetPlayersWS::~GetPlayersWS() {
}

WebServiceResult GetPlayersWS::getPlayersInCurrentMap() {
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

WebServiceResult GetPlayersWS::execute(HttpRequest& request, const std::string& baseEndpoint, const std::string& serviceEndpoint) throw(HttpException) {
	if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_GET && serviceEndpoint == "players") {
		return getPlayersInCurrentMap();
	}

	// Service not found
	throw HttpException(HttpException::SERVICE_NOT_EXISTS, "No player service found for the endpoint");
}

} /* namespace jkt */

