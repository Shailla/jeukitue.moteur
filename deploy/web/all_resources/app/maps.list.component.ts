import {Component} from 'angular2/core';
import {MapsService} from './maps.service';
import {HTTP_BINDINGS} from 'angular2/http';

@Component({
    selector: 'maps-list',
	providers: [MapsService,HTTP_BINDINGS],
	templateUrl: 'app/maps.list.component.html'
})
export class MapsListComponent implements OnInit {
	private _mapsService;
	public maps;

	constructor(_mapsService:MapsService) {
		this._mapsService = _mapsService;
	}
	
	ngOnInit() {
		this._mapsService.getMaps().subscribe(
			maps => this.maps = maps,
			console.error,
			() => console.log('Completed!')
		);
	}
}
