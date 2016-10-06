import { Injectable } from '@angular/core';

import {Player} from './player';
import {PLAYERS} from './mock-players';

@Injectable()
export class PlayerService {

	getPlayers(): Promise<Player[]> {
		return Promise.resolve(PLAYERS);
	}
}