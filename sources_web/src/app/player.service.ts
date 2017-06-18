import { Injectable } from '@angular/core';
import { Http } from '@angular/http';

import 'rxjs/add/operator/toPromise';

import { Player } from './player';
import { PLAYERS } from './mock-players';

@Injectable()
export class PlayerService {
  private playerServiceBaseUrl = 'http://localhost:20000/rest/player-service';

  private playersServiceUrl = this.playerServiceBaseUrl + '/players';		// Get players list in current Map

  constructor(private http: Http) { };

  getPlayers(): Promise<Player[]> {
    return this.http.get(this.playersServiceUrl)
      .toPromise()
      .then(response => response.json().players as Player[])
      .catch(this.handleError);
  }

  getPlayersSlowly(): Promise<Player[]> {
    return new Promise<Player[]>(resolve => setTimeout(resolve, 2000)) // delay 2 seconds
      .then(() => Promise.resolve(PLAYERS));
  }

  private handleError(error: any): Promise<any> {
    console.error('An error occurred', error);
    return Promise.reject(error.message || error);
  }
}
