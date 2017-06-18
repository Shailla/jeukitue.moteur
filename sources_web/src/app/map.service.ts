import { Injectable, Output, EventEmitter } from '@angular/core';
import { RequestOptions, Http, Headers } from '@angular/http';

import 'rxjs/add/operator/toPromise';

import { Map } from './map';
import { MapElement } from './MapElement';
import { MapGrapheElement } from './MapGrapheElement';
import { MAPS } from './mock-maps';
import { JSON_MAP } from './mock-map';

@Injectable()
export class MapService {
  private static mapServiceBaseUrl = 'http://localhost:20000/rest/map-service';

  private static mapsServiceUrl = MapService.mapServiceBaseUrl + '/maps';			// Get available Maps
  private static mapServiceUrl = MapService.mapServiceBaseUrl + '/map';			// Get the current Map graph
  private static mapElementServiceUrl = MapService.mapServiceBaseUrl + '/map/element';	// Get or update an element from current Map
  private static mapElementsServiceUrl = MapService.mapServiceBaseUrl + '/map/elements';	// Get or update several elements from current Map

  private static once = false;

  private headers = new Headers({ 'Content-Type': 'application/json' });


  @Output() mapElementUpdated = new EventEmitter<MapElement>();

  constructor(private http: Http) {
    if (MapService.once) {
      console.error('Multiple instanciations of singleton');
    }

    MapService.once = true;
  };

  // Lecture des Maps disponibles dans le jeu
  getMaps(): Promise<Map[]> {
    return this.http.get(MapService.mapsServiceUrl)
      .toPromise()
      .then(response => response.json().maps as Map[])
      .catch(this.handleError);
  }

  getMapsSlowly(): Promise<Map[]> {
    return new Promise<Map[]>(resolve => setTimeout(resolve, 1000)) // delay 2 seconds
      .then(() => Promise.resolve(MAPS));
  }

  // Lecture de la Map en cours dans le jeu
  getMapGraphe(): Promise<MapGrapheElement> {
    return this.http.get(MapService.mapServiceUrl)
      .toPromise()
      .then(response => response.json())
      .then(json => json.mapElement)
      .then(mapGrapheElement => new MapGrapheElement().fromJson(mapGrapheElement))
      .catch(this.handleError);
  }

  getMapGrapheSlowly(): Promise<MapGrapheElement> {
    return new Promise<MapGrapheElement>(resolve => setTimeout(resolve, 1000)) // delay 2 seconds
      .then(() => Promise.resolve(JSON_MAP))
      .then((json: any) => new MapGrapheElement().fromJson(json.mapElement))
      .catch(this.handleError);
  }

  // Lecture d'un élément de la Map en cours dans le jeu
  getMapElement(mapElementId: number): Promise<MapElement> {
    return this.http.get(MapService.mapElementServiceUrl + '/' + mapElementId)
      .toPromise()
      .then(response => response.json())
      .then(json => json.mapElement)
      .then(mapElement => new MapElement().fromJson(mapElement))
      .catch(this.handleError);
  }

  updateMapElement(mapElementId: number, mapElement: MapElement): Promise<MapElement> {
    const param = {};
    param['mapElement'] = mapElement;

    const body = JSON.stringify(param);
    console.log('REQUEST:' + body);

    return this.http.put(MapService.mapElementServiceUrl + '/' + mapElementId, body, { headers: this.headers })
      .toPromise()
      .then(response => {
        const json = response.json();
        const jsonMapElement = json.mapElement;
        this.mapElementUpdated.emit(new MapElement().fromJson(jsonMapElement));

        return mapElement;
      }
      )
      .catch(this.handleError);
  }

  updateMapElements(elementsToUpdate: any) {
    const param = {};
    param['mapElements'] = elementsToUpdate;

    const body = JSON.stringify(param);
    console.log('REQUEST:' + body);

    this.http.put(MapService.mapElementsServiceUrl, body, { headers: this.headers })
      .toPromise()
      .then(response => {
        const json = response.json();
        console.log('RESPONSE:' + JSON.stringify(json));
        json.mapElements.forEach(
          (mapElement: any) => this.mapElementUpdated.emit(mapElement)
        );
      }
      )
      .catch(this.handleError);
  }

  // Sauvegarde de la Map courante
  saveMap() {
    return this.http.post(MapService.mapServiceUrl, '', { headers: this.headers })
      .toPromise()
      .catch(this.handleError);
  }

  private handleError(error: any): Promise<any> {
    console.error('An error occurred', error);
    return Promise.reject(error.message || error);
  }
}
