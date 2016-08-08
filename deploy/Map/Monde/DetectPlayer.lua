
game = 0;

function onLoad()
	log("Version Lua A : " .. _VERSION)
	
	game = getGame();
	map = game:getMap();
	detector = map:createPlayerZoneDetector("idCoucou", 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	detector:setVisibility(true);
end

function eventManager(event)
	local actionId = event:getActionId();
	
	if actionId == 1120 then		-- PlayerDetectorActivated : Le joueur entre dans une zone de détection
		info = event:getInfo();
		playerId = info:getPlayerId();
		player = game:getPlayer(playerId);
		name = player:getName();
		logConsoleInfo("Le joueur '" .. name .. "(" .. playerId .. ")' entre dans la zone");
	
	elseif actionId == 1121 then	-- PlayerDetectorUnactivated : Le joueur sort d'une zone de détection
		info = event:getInfo();
		playerId = info:getPlayerId();
		player = game:getPlayer(playerId);
		name = player:getName();
		logConsoleInfo("Le joueur '" .. name .. "(" .. playerId .. ")' sort de la zone");
	end
end
