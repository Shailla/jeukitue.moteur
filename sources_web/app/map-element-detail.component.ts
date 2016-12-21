import { Component, Input } from '@angular/core';

import { MapElement } 		from './MapElement';

@Component({
	selector: 'map-element-detail',
	templateUrl: 'app/map-element-detail.component.html'
})
export class MapElementDetail {
	@Input() mapElementId: number;
}