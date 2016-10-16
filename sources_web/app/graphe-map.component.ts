import { Component, OnInit } from '@angular/core';

import { TreeViewComponent } from './tree-view.component';
import { MapService } from './map.service';
import { MapElement } from './mapElement';
import { Map } 		from './map';

@Component({
	selector: 	'my-graphe',
	template: 	`<h1 class="title" style="text-align: center;">Graphe Map courante</h1>
				<tree-view [items]="elements"></tree-view>`,
	providers:	[MapService]
})
export class GrapheMapComponent implements OnInit {

	elements: MapElement[];
	
	constructor(private mapService: MapService) {
		this.mapService.getMap().then(elements => this.elements = elements);
	}
	
	ngOnInit(): void {
	}
}