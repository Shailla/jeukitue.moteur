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

	// Hide or UnHide all checked elements
	onHideElements(hidden: boolean) {
		console.log("Hide or UnHide selected elements");
	
		let elementsToUpdate = [];
		this.changeElements(this.elements, "hidden", hidden, elementsToUpdate);
		
		console.log(elementsToUpdate);
		
		this.mapService.updateMapElements(elementsToUpdate);
	}
	
	// Highlight or UnHighlight all checked elements
	onHighlightElements(highlighted: boolean) {
		console.log("Highlight or UnHighlight selected elements");
		
		let elementsToUpdate = [];
		this.changeElements(this.elements, "highlighted", highlighted, elementsToUpdate);
		
		console.log(elementsToUpdate);
		
		this.mapService.updateMapElements(elementsToUpdate);
	}

	private changeElements(elements: MapGrapheElement[], param: string, state: boolean, elementsToUpdate) {
		elements.forEach(
			element => {
				if(element.checked) {
					let newElement = {};
					newElement["id"] = element.id;
					newElement[param] = state;
					elementsToUpdate.push(newElement);
				}
				
				this.changeElements(element.elements, param, state, elementsToUpdate);
			}
		);
	}
}