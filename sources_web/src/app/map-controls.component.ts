import { Component, Input } from '@angular/core';

import { MapService } from './map.service';

@Component({
  selector: 'app-map-controls',
  templateUrl: './map-controls.component.html'
})
export class MapControlsComponent {

  constructor(private mapService: MapService) {
  }

  onSaveMap() {
    console.log('Save Map');
    this.mapService.saveMap();
  }
}
