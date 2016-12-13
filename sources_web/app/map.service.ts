import { Injectable }    from '@angular/core';
import { Http } from '@angular/http';

import 'rxjs/add/operator/toPromise';

import { Map } 			from './map';
import { MapElement } 	from './mapElement';
import { MAPS } 		from './mock-maps';
import { JSON_MAP }		from './mock-map';

@Injectable()
export class MapService {
	private mapsServiceUrl = 'http://localhost:20000/rest/maps';
	private mapServiceUrl = 'http://localhost:20000/rest/map-graphe';
	
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
	getMap(): Promise<MapElement> {	
		return this.http.get(this.mapServiceUrl)
			.toPromise()
			.then(response => response.json())
			.then(json => json.mapElement)
			.then(mapElement => new MapElement().fromJson(mapElement))
			.catch(this.handleError); 
	}
	
	getMapSlowly(): Promise<MapElement> {
		return new Promise<MapElement>(resolve => setTimeout(resolve, 1000)) // delay 2 seconds
			.then(() => Promise.resolve(JSON_MAP))
			.then(json => json.mapElement)
			.then(mapElement => new MapElement().fromJson(mapElement))
			.catch(this.handleError);
	}

	
	private handleError(error: any): Promise<any> {
		console.error('An error occurred', error);
		return Promise.reject(error.message || error);
	}
}