/*
 * PlayersWebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#include <stdlib.h>

#include "reseau/web/json/JsonObject.h"
#include "reseau/web/HtmlServer.h"
#include "service/PlayersService.h"
#include "service/dto/PlayerInformationDto.h"

#include "reseau/web/service/PlayersWebService.h"

namespace jkt {

PlayersWebService::PlayersWebService() {
}

PlayersWebService::~PlayersWebService() {
}

WebServiceResult PlayersWebService::execute(const std::string& endpoint, const std::string& method) {
	JsonObject root;
	JsonList& players = root.addList("players");

	vector<PlayerInformationDto> dtos;
	PlayersService::getPlayers(dtos);

	for(PlayerInformationDto dto : dtos) {
		JsonObject& jsonPlayer = players.addObject();
		jsonPlayer.addString("name", dto.getName());
	}

	string json = root.toString();

	WebServiceResult result;
	result._contentSize = json.size();
	result._content = malloc(json.size());
	json.copy((char*)result._content, json.size());
	result._contentType = HtmlServer::HTTP_CONTENT_TYPE_JSON;
	result._status = HtmlServer::HTTP_RESPONSE_200;

	return result;
}

} /* namespace jkt */

