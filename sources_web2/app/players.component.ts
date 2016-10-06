import { Component } from '@angular/core';
import { OnInit } from '@angular/core';

import { Player } from './player';
import { PlayerService } from './player.service';

@Component({
  selector: 	'my-players',
  templateUrl: 	'app/players.component.html',
  providers:	[PlayerService]
})
export class PlayersComponent implements OnInit {
	players: Player[];
	
	constructor(private playerService: PlayerService) {
	}
	
	ngOnInit(): void {
		this.getPlayers();
	}
	
	getPlayers(): void {
		this.playerService.getPlayers().then(players => this.players = players);
	}
}