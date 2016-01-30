import {Injectable} from 'angular2/core';
import {Http} from 'angular2/http';
import 'rxjs/Rx';

@Injectable()
export class PlayersService {
	private _http;
	public players;
	
	constructor(_http:Http) {
		this._http = _http;
		console.log('Players service created.', _http);
		this.players = _http.get('users.json').map(response  => response.json());
	}
	
	getPlayers() {
		return this._http.get('users.json').map(response  => response.json());
	}
}
