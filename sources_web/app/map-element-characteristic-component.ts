import { Component, Input } from '@angular/core';

import { MapGrapheElementCharacteristic } 		from './MapGrapheElementCharacteristic';

@Component({
	selector: 'map-graph-element-characteristic',
	templateUrl: 'app/map-graph-element-characteristic.component.html'
})
export class MapGrapheElementCharacteristicComponent {
	@Input() characteristic: MapGrapheElementCharacteristic;
}