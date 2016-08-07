
function onLoad()
	log("Version Lua A : " .. _VERSION)
	
	detector = Map:createPlayerZoneDetector("idCoucou", 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	detector:setVisibility(true);
end

function eventManager(event)
	local actionId = event:getActionId();
	
	if actionId == 1120 then		-- PlayerDetectorActivated : Le joueur entre dans une zone de détection
		player = event:getInfo();
		logConsoleInfo("Le joueur '" .. player:getPlayerId() .. "' entre dans la zone");
	
	elseif actionId == 1121 then	-- PlayerDetectorUnactivated : Le joueur sort d'une zone de détection
		player = event:getInfo();
		logConsoleInfo("Le joueur '" .. player:getPlayerId() .. "' sort de la zone");
	end
end
