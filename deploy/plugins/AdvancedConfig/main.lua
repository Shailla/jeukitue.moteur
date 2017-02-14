
-- Variables globales
window = 0
checkboxSkinJoueurVisibility = 0;
checkboxHitboxJoueurVisibility = 0;
checkboxSolidboxJoueurVisibility = 0;
checkboxAxesMeterVisibility = 0;
checkboxCubicMeterVisibility = 0;

appliquerJoueurButton = 0;
reinitialiserJoueurButton = 0;
closeButton = 0;

function onLoad()
	log("Version Lua : " .. _VERSION)

	-- Ouverture fenêtre
	window = Window();
	window:setTitle("Configuration avancée");
	window:setSize(400, 400);
	window:setPosition(0, 0);
	local notebook = window:addNotebook();

	-- Onglet joueur
	local tabJoueur = notebook:addTab("Joueur");
	local boxJoueur = tabJoueur:addBoxVert();
	checkboxSkinJoueurVisibility = boxJoueur:addCheckbox("Voir le skin du joueur");
	checkboxHitboxJoueurVisibility = boxJoueur:addCheckbox("Voir la hit box des joueurs");
	checkboxSolidboxJoueurVisibility = boxJoueur:addCheckbox("Voir les contours solides des joueurs");
	checkboxAxesMeterVisibility = boxJoueur:addCheckbox("Voir les axes");
	checkboxCubicMeterVisibility = boxJoueur:addCheckbox("Voir un cube d'un mètre");
	
	loadJoueurConfiguration();	-- Chargement des valeurs courantes
	local boxBoutonsJoueur = boxJoueur:addBoxHoriz();
	appliquerJoueurButton = boxBoutonsJoueur:addButton("Appliquer");
	reinitialiserJoueurButton = boxBoutonsJoueur:addButton("Reinitialiser");
	
	closeButton = boxJoueur:addButton("Fermer");

	-- Onglet caméra
	local tabCamera = notebook:addTab("Camera");
	local boxCamera = tabCamera:addBoxVert();
	boxCamera:addNumeric("Hauteur", "pixels");
	boxCamera:addNumeric("Recul", "pixels");
	boxCamera:addNumeric("Lateral", "pixels");

	local drivers = getAvailableAudioDrivers();
	log("Liste des drivers :");
	for k,v in pairs(drivers) do
		log("  "..k.." : "..v);
	end
	
	window:hide();
end

-- Lit la configuration joueur par défaut
function loadJoueurConfiguration()
	checkboxSkinJoueurVisibility:setValue(getConfigurationParameter("SKIN_VISIBILITY"));
	checkboxHitboxJoueurVisibility:setValue(getConfigurationParameter("PLAYER_HITBOX_VISIBILITY"));
	checkboxSolidboxJoueurVisibility:setValue(getConfigurationParameter("PLAYER_SOLIDBOX_VISIBILITY"));
	checkboxAxesMeterVisibility:setValue(getConfigurationParameter("AXES_METER_VISIBILITY"));
	checkboxCubicMeterVisibility:setValue(getConfigurationParameter("CUBIC_METER_VISIBILITY"));
end

-- Enregistre la configuration joueur
function appliqueJoueurConfiguration()
	setConfigurationParameter("SKIN_VISIBILITY", checkboxSkinJoueurVisibility:getValue());
	setConfigurationParameter("PLAYER_HITBOX_VISIBILITY", checkboxHitboxJoueurVisibility:getValue());
	setConfigurationParameter("PLAYER_SOLIDBOX_VISIBILITY", checkboxSolidboxJoueurVisibility:getValue());
	setConfigurationParameter("AXES_METER_VISIBILITY", checkboxAxesMeterVisibility:getValue());
	setConfigurationParameter("CUBIC_METER_VISIBILITY", checkboxCubicMeterVisibility:getValue());
end

function eventManager(event)
	local actionId = event:getActionId();
	log("Event actionId=" .. actionId);
	
	if actionId == 1119 then		-- Show advanced configuration window
		log("SHOW");
		window:show();
		
	elseif actionId == 1004 then		-- Widget event
		local source = event:getSource();

		-- Enregistrement de la configuration du joueur
		if source == appliquerJoueurButton then
			log("Enregistrement de la configuration du joueur");
			appliqueJoueurConfiguration();
			saveConfiguration();
			
		-- Lecture de la configuration joueur par défaut
		elseif source == reinitialiserJoueurButton then
			log("Lecture de la configuration joueur par défaut");
			loadJoueurConfiguration();
			
		-- Fermer la fenêtre
		elseif source == closeButton then
			log("HIDE");
			window:hide();
			
		-- Evênement non-pris en compte
		else
			log("Evenement ignoré ("..event:__tostring()..")");
		end
	end
end
