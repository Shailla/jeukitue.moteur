
function onLoad()
	log("Version Lua A : " .. _VERSION)
	
	detector = Map:createPlayerZoneDetector("idCoucou", 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	detector:setVisibility(true);
	logConsoleInfo("Salut");
end

function eventManager(event)
	local type = event:getType();
		
	-- Action event
	if type == 1 then
		local actionId = event:getActionId();
		if actionId == 1020 then	-- PlayerDetectorActivated : Le joueur a été détecté dans une zone de détection
			logConsoleInfo("Le joueur entre dans la zone");
		elseif actionId == 1021 then
			logConsoleInfo("Le joueur sort de la zone");
		end
	end
end
