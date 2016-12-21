import { Component, Input } from '@angular/core';

import { GrapheMapComponent } 		from './graphe-map.component';
import { MapGrapheElement } 		from './MapGrapheElement';

@Component({
	selector: 'tree-view',
	templateUrl: 'app/tree-view.component.html'
})
export class TreeViewComponent {
	@Input() grapheMapComponent: GrapheMapComponent;
	@Input() items: Array<MapGrapheElement>;
}