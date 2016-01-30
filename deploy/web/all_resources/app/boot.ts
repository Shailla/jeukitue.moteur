import {bootstrap} from 'angular2/platform/browser';
import {HTTP_BINDINGS} from 'angular2/http';
import {ROUTER_PROVIDERS} from 'angular2/router';

// Services
import {MapsService} from './maps.service';
import {PlayersService} from './players.service';
// Components
import {DashboardComponent} from './dashboard.component';
import {MapsListComponent} from './maps.list.component';
import {PlayersListComponent} from './players.list.component';


bootstrap(DashboardComponent, [ROUTER_PROVIDERS]).catch(err => console.error(err));
bootstrap(MapsListComponent, [MapsService, HTTP_BINDINGS]).catch(err => console.error(err));
bootstrap(PlayersListComponent, [PlayersService, HTTP_BINDINGS]).catch(err => console.error(err));
