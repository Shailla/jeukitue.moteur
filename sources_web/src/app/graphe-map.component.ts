import { Component } from '@angular/core';

import { TreeViewComponent } from './tree-view.component';
import { MapService } from './map.service';
import { MapGrapheElement } from './mapGrapheElement';
import { MapElement } from './mapElement';
import { Map } from './map';

@Component({
  selector: 'app-map-graphe',
  templateUrl: './graphe-map.component.html'
})
export class GrapheMapComponent {
  elements: MapGrapheElement[];
  selectedMapElementId: number;

  constructor(private mapService: MapService) {
    this.mapService.getMapGraphe().then(element => this.elements = [element]);

    this.mapService.mapElementUpdated.subscribe((mapElement: any) => this.onMapElementUpdated(mapElement));
  }

  onSelectMapElement(mapGrapheElement: MapGrapheElement) {
    this.selectedMapElementId = mapGrapheElement.id;
  }

  private onMapElementUpdated(mapElement: MapElement) {
    if (this.elements !== undefined) {
      console.log('graphe-map received "onMapElementUpdated" event');
      this.elements.forEach(element => element.updateMapElement(mapElement));
    } else {
      console.log('graphe-map received "onMapElementUpdated" event, ignored because elements are undefined');
    }
  }

  // Hide or UnHide all checked elements
  onHideElements(hidden: boolean) {
    console.log('Hide or UnHide selected elements');

    const elementsToUpdate: any[] = [];
    this.changeElements(this.elements, 'hidden', hidden, elementsToUpdate);

    console.log(elementsToUpdate);

    this.mapService.updateMapElements(elementsToUpdate);
  }

  // Highlight or UnHighlight all checked elements
  onHighlightElements(highlighted: boolean) {
    console.log('Highlight or UnHighlight selected elements');

    const elementsToUpdate: any[] = [];
    this.changeElements(this.elements, 'highlighted', highlighted, elementsToUpdate);

    console.log(elementsToUpdate);

    this.mapService.updateMapElements(elementsToUpdate);
  }

  private changeElements(elements: MapGrapheElement[], paramName: string, state: boolean, elementsToUpdate: any[]) {
    elements.forEach(
      element => {
        if (element.checked) {
          const newElement = {'id': element.id, 'caracs': {} };
          newElement.caracs[paramName] = {'caracValue': state};
          elementsToUpdate.push(newElement);
        }

        this.changeElements(element.elements, paramName, state, elementsToUpdate);
      }
    );
  }
}
