import {MapElement} from './mapElement';

let item11 = new MapElement("Item11",	[]						, ""	);
let item12 = new MapElement("Geo12", 	[]						, ""	);
let item13 = new MapElement("Item13",	[]						, "geo"	);
let parent1 = new MapElement("Parent1",	[item11,item12,item13]	, ""	);
let item21 = new MapElement("Item21",	[]						, ""	);
let item22 = new MapElement("Item22",	[]						, "geo"	);
let item23 = new MapElement("Item23",	[]						, ""	);
let parent2 = new MapElement("Parent2",	[item21,item22,item23]	, ""	);
let item1 = new MapElement("Item1",		[]						, ""	);
let item2 = new MapElement("Item2",		[]						, ""	);

export const MAP: MapElement[] = [parent1, parent2, item1, item2];
