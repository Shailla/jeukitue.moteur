import {Component} from 'angular2/core';
import {PlayersService} from './players.service';
import {HTTP_BINDINGS} from 'angular2/http';

@Component({
    selector: 'players-list',
	providers: [PlayersService,HTTP_BINDINGS],
	templateUrl: 'app/players.list.component.html'
})
export class PlayersListComponent implements OnInit {
	private _playersService;
	public players;

	constructor(_playersService:PlayersService) {
		this._playersService = _playersService;
	}
	
	ngOnInit() {
		this._playersService.getPlayers().subscribe(
			players => this.players = players,
			console.error,
			() => console.log('Completed!')
		);
	}
}
