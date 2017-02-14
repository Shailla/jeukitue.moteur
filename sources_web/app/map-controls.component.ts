import { Component, Input } from '@angular/core';

import { MapService } 		from './map.service';

@Component({
	selector: 		'map-controls',
	templateUrl: 	'app/map-controls.component.html'
})
export class MapControls {
	
	constructor(private mapService: MapService) {
	}
	
	onSaveMap() {
		console.log("Save Map");
		this.mapService.saveMap();
	}
}