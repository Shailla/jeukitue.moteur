import { Component, Input } from '@angular/core';

import { MapGrapheElementCarac } from './MapGrapheElementCarac';

@Component({
  selector: 'app-map-element-carac',
  templateUrl: './map-element-carac-detail.component.html'
})
export class MapGrapheElementCaracComponent {
  @Input() carac: MapGrapheElementCarac;
}
