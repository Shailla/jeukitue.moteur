import {Injectable} from 'angular2/core';
import {Http} from 'angular2/http';
import 'rxjs/Rx';

@Injectable()
export class MapsService {
	private _http;
	public maps;
	
	constructor(_http:Http) {
		this._http = _http;
		console.log('Maps service created.', _http);
		this.maps = _http.get('maps.json').map(response  => response.json());
	}
	
	getMaps() {
		return this._http.get('maps.json').map(response  => response.json());
	}
}
