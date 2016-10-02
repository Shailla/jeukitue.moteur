
-- Constantes
windowWeight = 200
windowHeight = 300

-- Variables globales
window = 0

sceneButton = 0
multiButton = 0
configurationButton = 0
navigatorButton = 0
aboutButton = 0
debugButton = 0
quitMapButton = 0
quitButton = 0

function onLoad()
	log("Version Lua : " .. _VERSION)

	-- ----------------------------------------
	-- Creation IHM
	-- ----------------------------------------

	-- Ouverture fenêtre
	window = Window();
	window:setTitle("Menu principal");
	
	-- Combos	
	sceneButton = window:addButton("Ouvrir une scene");
	sceneButton:expandHoriz();
	
	multiButton = window:addButton("Multi-joueurs");
	multiButton:expandHoriz();
	
	configurationButton = window:addButton("Configuration");
	configurationButton:expandHoriz();

	navigatorButton = window:addButton("Navigateur");
	navigatorButton:expandHoriz();
	
	aboutButton = window:addButton("A propos");
	aboutButton:expandHoriz();
	
	debugButton = window:addButton("Debug");
	debugButton:expandHoriz();
		
	-- Boutons
	window:addSeparator();

	backButton = window:addButton("Retour au jeu");
	backButton:expandHoriz();

	quitMapButton = window:addButton("Quitter la partie");
	quitMapButton:expandHoriz();
	
	quitButton = window:addButton("Quitter");
	quitButton:expandHoriz();
	
	-- Affichage de la fenêtre
	window:setSize(windowWeight, windowHeight);
	
	local screenSizeWeight, screenSizeHeight = getScreenSize();
	window:setPosition((screenSizeWeight - windowWeight) / 2, (screenSizeHeight - windowHeight) / 2);
	
	window:hide();
end

function eventManager(event)
	local actionId = event:getActionId();
	log("Event actionId=" .. actionId);
	
	if actionId == 1002 then		-- ShowMenuAction
		window:show();

	elseif actionId == 1001 then	-- HideMenuAction
		window:hide();
			
	elseif actionId == 1004 then		-- Widget event
		log("Widget event");	
		local source = event:getSource();
		
		if source == sceneButton then
			log("Push event 1116");
			window:hide();
			pushEvent(1116);	-- ShowMenuOpenSceneAction
			
		elseif source == multiButton then
			window:hide();
			pushEvent(1105);	-- ShowMultijoueursMenuAction
		
		elseif source == configurationButton then
			window:hide();
			pushEvent(1104);	-- ShowConfigurationMenuAction
			
		elseif source == quitMapButton then
			window:hide();
			pushEvent(1117);	-- QuitGameAction

		elseif source == navigatorButton then
			pushEvent(1122);
			
		elseif source == aboutButton then
			window:hide();
			pushEvent(10001);
			
		elseif source == debugButton then
			window:hide();
			pushEvent(1114);	-- ShowDebugViewAction
			
		elseif source == quitButton then
			window:hide();
			pushEvent(1118);	-- QuitAction
			
		elseif source == backButton then
			window:hide();
			
		-- Evênement non-pris en compte
		else
			log("Evenement ignoré (" .. event:__tostring() .. ")");
		end
	end
end
