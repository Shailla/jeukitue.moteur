import { Injectable }    from '@angular/core';
import { Headers, Http } from '@angular/http';

import 'rxjs/add/operator/toPromise';

import {Player} from './player';
import {PLAYERS} from './mock-players';

@Injectable()
export class PlayerService {
	private playersServiceUrl = 'rest/players';
	
	constructor(private http: Http) {};

	getPlayers(): Promise<Player[]> {
		// return Promise.resolve(PLAYERS);
		
		return this.http.get(this.playersServiceUrl)
               .toPromise()
               .then(response => response.json().data as Player[])
               .catch(this.handleError);
	}
	
	private handleError(error: any): Promise<any> {
		console.error('An error occurred', error);
		return Promise.reject(error.message || error);
	}
}