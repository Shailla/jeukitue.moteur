import { Component, OnInit } from '@angular/core';

import { Map } from './map';
import { MapService } from './map.service';

@Component({
  selector: 	'my-maps',
  templateUrl: 	'app/maps.component.html'
})
export class MapsComponent implements OnInit {
	maps: Map[];
	
	constructor(private mapService: MapService) {
	}
	
	ngOnInit(): void {
		this.getMaps();
	}
	
	getMaps(): void {
		this.mapService.getMaps().then(maps => 	{
														this.maps = maps;
														console.log('Maps JSON response : ', this.maps);
												});
	}
}