import { Injectable }    from '@angular/core';
import { Http } from '@angular/http';

import 'rxjs/add/operator/toPromise';

import { Map } 				from './map';
import { MapGrapheElement } from './MapGrapheElement';
import { MAPS } 			from './mock-maps';
import { JSON_MAP }			from './mock-map';

@Injectable()
export class MapService {
	private mapServiceBaseUrl = 'http://localhost:20000/rest/map-service';
	
	private mapsServiceUrl = this.mapServiceBaseUrl + '/maps';		// Get available Maps
	private mapServiceUrl = this.mapServiceBaseUrl + '/map-graphe';			// Get the current Map graph
	
	constructor(private http: Http) {};

	// Lecture des Maps disponibles dans le jeu	
	getMaps(): Promise<Map[]> {	
		return this.http.get(this.mapsServiceUrl)
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
		return this.http.get(this.mapServiceUrl)
			.toPromise()
			.then(response => response.json())
			.then(json => json.mapElement)
			.then(mapGrapheElement => new MapGrapheElement().fromJson(mapGrapheElement))
			.catch(this.handleError); 
	}
	
	getMapGrapheSlowly(): Promise<MapGrapheElement> {
		return new Promise<MapGrapheElement>(resolve => setTimeout(resolve, 1000)) // delay 2 seconds
			.then(() => Promise.resolve(JSON_MAP))
			.then(json => json.mapElement)
			.then(mapGrapheElement => new MapGrapheElement().fromJson(mapGrapheElement))
			.catch(this.handleError);
	}

	
	private handleError(error: any): Promise<any> {
		console.error('An error occurred', error);
		return Promise.reject(error.message || error);
	}
}