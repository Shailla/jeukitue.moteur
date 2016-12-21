import { Component, OnInit } from '@angular/core';

import { TreeViewComponent } 	from './tree-view.component';
import { MapService } 			from './map.service';
import { MapGrapheElement } 	from './mapGrapheElement';
import { MapElement } 			from './mapElement';
import { Map } 					from './map';

@Component({
	selector: 		'my-graphe',
	templateUrl:	'app/graphe-map.component.html',
	providers:		[MapService]
})
export class GrapheMapComponent implements OnInit {

	elements: MapGrapheElement[];
	selectedMapElementId: number;
	
	constructor(private mapService: MapService) {
		this.mapService.getMapGraphe().then(element => this.elements = [element]);
	}
	
	ngOnInit(): void {
	}
	
	onSelectMapElement(mapGrapheElement: MapGrapheElement) {
		this.selectedMapElementId = mapGrapheElement.id;
	}
}