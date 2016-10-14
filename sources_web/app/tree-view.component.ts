import { Component, Input } from '@angular/core';

import { MapElement } 		from './mapElement';

@Component({
	selector: 'tree-view',
	templateUrl: 'app/tree-view.component.html'
})
export class TreeViewComponent {
	@Input() items: Array<MapElement>;
}