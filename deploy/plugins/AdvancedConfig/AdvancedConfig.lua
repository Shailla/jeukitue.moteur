
-- Variables globales
checkboxSkinJoueurVisibility = 0;
checkboxOutlineJoueurVisibility = 0;
appliquerJoueurButton = 0;
reinitialiserJoueurButton = 0;

function onLoad()
	-- the first program in every language
	log("Hello world, from " .. _VERSION)

	local window = Window();
	window:setTitle("Configuration avancee");
	window:setSize(200, 200);
	window:setPosition(0, 0);
	local notebook = window:addNotebook();

	-- Onglet joueur
	local tabJoueur = notebook:addTab("Joueur");
	local boxJoueur = tabJoueur:addBoxVert();
	checkboxSkinJoueurVisibility = boxJoueur:addCheckbox("Voir le skin du joueur");
	checkboxOutlineJoueurVisibility = boxJoueur:addCheckbox("Voir les contours solides des joueurs");
	loadJoueurConfiguration();	-- Chargement des valeurs courantes
	local boxBoutonsJoueur = boxJoueur:addBoxHoriz();
	appliquerJoueurButton = boxBoutonsJoueur:addButton("Appliquer");
	reinitialiserJoueurButton = boxBoutonsJoueur:addButton("Reinitialiser");

	-- Onglet caméra
	local tabCamera = notebook:addTab("Camera");
	local boxCamera = tabCamera:addBoxVert();
	boxCamera:addNumeric("...", "pixels");
	boxCamera:addNumeric("...", "pixels");

	window:show();
end

-- Lit la configuration joueur par défaut
function loadJoueurConfiguration()
	checkboxSkinJoueurVisibility:setValue(isPlayerSkinVisible());
	checkboxOutlineJoueurVisibility:setValue(isPlayerOutlineVisible());
end

-- Enregistre la configuration joueur
function saveJoueurConfiguration()
	setPlayerSkinVisibility(checkboxSkinJoueurVisibility:getValue());
	setPlayerOutlineVisibility(checkboxOutlineJoueurVisibility:getValue());
end

function eventManager(event)
	-- Enregistrement de la configuration du joueur
	if event == appliquerJoueurButton then
		log("Enregistrement de la configuration du joueur");
		saveJoueurConfiguration();
		
	-- Lecture de la configuration joueur par défaut
	elseif event == reinitialiserJoueurButton then
		log("Lecture de la configuration joueur par défaut");
		loadJoueurConfiguration();
		
	-- Evénement non-pris en compte
	else
		log("Evenement inconnu");
	end
end
