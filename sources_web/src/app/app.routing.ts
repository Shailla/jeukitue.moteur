import { ModuleWithProviders } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { DashboardComponent }	from './dashboard.component';
import { PlayersComponent } from './players.component';
import { MapsComponent } from './maps.component';
import { GrapheMapComponent }	from './graphe-map.component';

const appRoutes: Routes = [
 {	path: '',				redirectTo: '/dashboard',	pathMatch: 'full' },
 {	path: 'dashboard',    	component: DashboardComponent },
 {	path: 'players',    	component: PlayersComponent },
 {	path: 'maps',    		component: MapsComponent },
 {	path: 'graphe-map',    	component: GrapheMapComponent }
];

export const routing: ModuleWithProviders = RouterModule.forRoot(appRoutes);
