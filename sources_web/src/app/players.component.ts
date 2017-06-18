import { Component, OnInit } from '@angular/core';

import { Player } from './player';
import { PlayerService } from './player.service';

@Component({
  selector: 'app-players',
  templateUrl: 'players.component.html'
})
export class PlayersComponent implements OnInit {
  players: Player[];

  constructor(private playerService: PlayerService) {
  }

  ngOnInit(): void {
    this.getPlayers();
  }

  getPlayers(): void {
    this.playerService.getPlayers().then(players => {
      this.players = players;
      console.log('Players JSON response : ', this.players);
    }
    );
  }
}
