import { Component, Input } from '@angular/core';

import { MapElement } 		from './MapElement';
import { MapService } 		from './map.service';

@Component({
	selector: 'map-element-detail',
	templateUrl: 'app/map-element-detail.component.html',
	providers:		[MapService]
})
export class MapElementDetail {
	@Input() mapElementId: number;
	mapElement: MapElement;
	
	constructor(private mapService: MapService) {
	}
	
	ngOnChanges(): void {
		console.log("ngOnChange => " + this.mapElementId);
		this.mapService.getMapElement(this.mapElementId).then(mapElement => this.mapElement = mapElement);
	}
}