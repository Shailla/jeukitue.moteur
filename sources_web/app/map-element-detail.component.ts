import { Component, Input } from '@angular/core';

import 'rxjs/add/operator/toPromise';

import { MapElement } 		from './MapElement';
import { MapService } 		from './map.service';

@Component({
	selector: 		'map-element-detail',
	templateUrl: 	'app/map-element-detail.component.html'
})
export class MapElementDetail {
	@Input() mapElementId: number;
	mapElement: MapElement;
	
	constructor(private mapService: MapService) {
		this.mapService.mapElementUpdated.subscribe(mapElement => this.onMapElementUpdated(mapElement));
	}
	
	ngOnChanges(): void {
		if(this.mapElementId != undefined) {
			console.log("ngOnChange 1 => " + this.mapElementId);
			
			 this.mapService.getMapElement(this.mapElementId).then(
				mapElement => this.mapElement = mapElement
			 );
		}
	}
	
	onSubmit() {
		console.log("Submit map element detail");
		this.mapService.updateMapElement(this.mapElementId, this.mapElement);
	}
	
	private onMapElementUpdated(mapElement: MapElement) {
		console.log("map-element-detail received 'mapElementUpdated' event");
		this.mapElement = mapElement;
	}
}