import { Component } from '@angular/core';

import { TreeViewComponent } 	from './tree-view.component';
import { MapService } 			from './map.service';
import { MapGrapheElement } 	from './mapGrapheElement';
import { MapElement } 			from './mapElement';
import { Map } 					from './map';

@Component({
	selector: 		'map-graphe',
	templateUrl:	'app/graphe-map.component.html'
})
export class GrapheMapComponent {
	elements: MapGrapheElement[];
	selectedMapElementId: number;
	
	constructor(private mapService: MapService) {
		this.mapService.getMapGraphe().then(element => this.elements = [element]);
		
		this.mapService.mapElementUpdated.subscribe(mapElement => this.onMapElementUpdated(mapElement));
	}
	
	onSelectMapElement(mapGrapheElement: MapGrapheElement) {
		this.selectedMapElementId = mapGrapheElement.id;
	}
	
	private onMapElementUpdated(mapElement: MapElement) {
		console.log("graphe-map received 'mapElementUpdated' event");
		this.elements.forEach(element => element.updateMapElement(mapElement));
	}
}