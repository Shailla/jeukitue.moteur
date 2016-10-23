import {MapElement} from './mapElement';

let item11 = new MapElement(		"light",	1,	"Item11",		[]							);
let item12 = new MapElement(		"light",	2,	"Geo12", 		[]							);
let item13 = new MapElement(		"geo",		3,	"Item13",		[]							);
let parent1 = new MapElement(		"map",		4,	"Parent1",		[item11,item12,item13]		);

let item211 = new MapElement(		"light",	5,	"Item211",		[]							);
let item212 = new MapElement(		"geo",		6,	"Item212",		[]							);
let item213 = new MapElement(		"geo",		7,	"Item213",		[]							);
let subParent21 = new MapElement(	"map",		8,	"subParent21",	[item211, item212, item213]	);
let item22 = new MapElement(		"light",	9,	"Item22",		[]							);
let item23 = new MapElement(		"geo",		10,	"Item23",		[]							);
let parent2 = new MapElement(		"map",		11,	"Parent2",		[subParent21,item22,item23]	);

let item1 = new MapElement(			"geo",		12,	"Item1",		[]							);
let item2 = new MapElement(			"geo",		13,	"Item2",		[]							);

export const MAP: MapElement[] = [parent1, parent2, item1, item2];

