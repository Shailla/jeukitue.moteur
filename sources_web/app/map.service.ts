import { Injectable }    from '@angular/core';
import { Http } from '@angular/http';

import 'rxjs/add/operator/toPromise';

import {Map} from './map';
import {MAPS} from './mock-maps';

@Injectable()
export class MapService {
	private mapsServiceUrl = 'http://localhost:20000/rest/maps';
	
	constructor(private http: Http) {};

	getMaps(): Promise<Map[]> {	
		return this.http.get(this.mapsServiceUrl)
				.toPromise()
				.then(response => response.json().maps as Map[])
				.catch(this.handleError);
	}
	
	getMapsSlowly(): Promise<Map[]> {
		return new Promise<Map[]>(resolve => setTimeout(resolve, 2000)) // delay 2 seconds
				.then(() => Promise.resolve(MAPS));
  }
	
	private handleError(error: any): Promise<any> {
		console.error('An error occurred', error);
		return Promise.reject(error.message || error);
	}
}