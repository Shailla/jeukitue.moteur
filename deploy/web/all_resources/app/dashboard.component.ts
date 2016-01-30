import {Component} from 'angular2/core';
import {RouteConfig, ROUTER_DIRECTIVES} from 'angular2/router';
import {MapsListComponent} from './maps.list.component';
import {PlayersListComponent} from './players.list.component';

@Component({
    selector: 'dashboard',
	directives: [ROUTER_DIRECTIVES],
	templateUrl: 'app/dashboard.component.html'
})
@RouteConfig([ 
  {path: '/maps-list', name: 'MapsList', component: MapsListComponent},
  {path: '/players-list', name: 'PlayersList', component: PlayersListComponent}
])
export class DashboardComponent {
}
