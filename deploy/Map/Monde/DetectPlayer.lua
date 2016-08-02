
function onLoad()
	log("Version Lua A : " .. _VERSION)
	
	detector = Map:createPlayerZoneDetector("idCoucou", 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	detector:setVisibility(true);
end

function eventManager(event)
	local type = event:getType();
		
	-- Action event
	if type == 1 then
		local actionId = event:getActionId();
		
		if actionId == 1020 then		-- PlayerDetectorActivated : Le joueur entre dans une zone de détection
			-- player = event:getSource();
			-- logConsoleInfo("Le joueur '" .. player:getName() .. "' entre dans la zone");
			logConsoleInfo("Le joueur entre dans la zone");
		
		elseif actionId == 1021 then	-- PlayerDetectorUnactivated : Le joueur sort d'une zone de détection
			-- player = event:getSource();
			-- logConsoleInfo("Le joueur '" .. player:getName() .. "' sort de la zone");
			logConsoleInfo("Le joueur sort de la zone");
		end
	end
end
