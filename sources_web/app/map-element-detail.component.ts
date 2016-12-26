import { Component, Input } from '@angular/core';

import 'rxjs/add/operator/toPromise';

import { MapElement } 		from './MapElement';
import { MapService } 		from './map.service';

@Component({
	selector: 		'map-element-detail',
	templateUrl: 	'app/map-element-detail.component.html',
	providers:		[MapService]
})
export class MapElementDetail {
	@Input() mapElementId: number;
	mapElement: Promise<MapElement>;
	
	constructor(private mapService: MapService) {
	}
	
	ngOnChanges(): void {
		if(this.mapElementId != undefined) {
			console.log("ngOnChange 1 => " + this.mapElementId);
			
			this.mapElement = this.mapService.getMapElement(this.mapElementId);
		}
	}
}