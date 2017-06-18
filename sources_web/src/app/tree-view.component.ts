import { Component, Input } from '@angular/core';

import { GrapheMapComponent } from './graphe-map.component';
import { MapGrapheElement } from './MapGrapheElement';

@Component({
  selector: 'app-tree-view',
  templateUrl: 'tree-view.component.html'
})
export class TreeViewComponent {
  @Input() grapheMapComponent: GrapheMapComponent;
  @Input() items: Array<MapGrapheElement>;
}
