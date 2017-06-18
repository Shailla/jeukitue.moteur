import { Component, Input, OnChanges } from '@angular/core';

import 'rxjs/add/operator/toPromise';

import { MapElement } from './MapElement';
import { MapService } from './map.service';
import { stringify } from 'querystring';

@Component({
  selector: 'app-map-element-detail',
  templateUrl: './map-element-detail.component.html'
})
export class MapElementDetailComponent implements OnChanges {
  @Input() mapElementId: number;
  mapElement: MapElement;

  constructor(private mapService: MapService) {
    this.mapService.mapElementUpdated.subscribe((mapElement: any) => this.onMapElementUpdated(mapElement));
  }

  ngOnChanges(): void {
    if (this.mapElementId !== undefined) {
      console.log('ngOnChange 1 => ' + this.mapElementId);

      this.mapService.getMapElement(this.mapElementId).then(
        mapElement => {
          this.mapElement = mapElement;
          console.log('get done => ' + this.mapElementId + stringify(this.mapElement));
        }
      );
    }
  }

  onSubmit() {
    console.log('Submit map element detail');
    this.mapService.updateMapElement(this.mapElementId, this.mapElement);
  }

  private onMapElementUpdated(mapElement: MapElement) {
    console.log('map-element-detail received "mapElementUpdated" event');
    this.mapElement = mapElement;
  }
}
