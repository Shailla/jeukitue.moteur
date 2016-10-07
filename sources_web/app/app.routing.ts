import { ModuleWithProviders }  from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { DashboardComponent }	from './dashboard.component';
import { PlayersComponent }		from './players.component';

const appRoutes: Routes = [
	{	path: '',				redirectTo: '/dashboard',	pathMatch: 'full'	},
	{	path: 'dashboard',    	component: DashboardComponent	},
	{	path: 'players',    	component: PlayersComponent	}
];

export const routing: ModuleWithProviders = RouterModule.forRoot(appRoutes);