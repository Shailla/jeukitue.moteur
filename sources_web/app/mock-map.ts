import {MapElement} from './mapElement';

let item11 = new MapElement("Item11",	[]									, "light"	);
let item12 = new MapElement("Geo12", 	[]									, "light"	);
let item13 = new MapElement("Item13",	[]									, "geo"	);
let parent1 = new MapElement("Parent1",	[item11,item12,item13]				, "map"	);

let item211 = new MapElement("Item211",	[]									, "light"	);
let item212 = new MapElement("Item212",	[]									, "geo"	);
let item213 = new MapElement("Item213",	[]									, "geo"	);
let subParent21 = new MapElement("subParent21",	[item211, item212, item213]	, "map"	);
let item22 = new MapElement("Item22",	[]									, "light"	);
let item23 = new MapElement("Item23",	[]									, "geo"	);
let parent2 = new MapElement("Parent2",	[subParent21,item22,item23]			, "map"	);

let item1 = new MapElement("Item1",		[]									, "geo"	);
let item2 = new MapElement("Item2",		[]									, "geo"	);

export const MAP: MapElement[] = [parent1, parent2, item1, item2];
