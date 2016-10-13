import { NgModule }      from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpModule }    from '@angular/http';

import { routing } 				from './app.routing';
import { DashboardComponent }   from './dashboard.component';
import { PlayersComponent }   	from './players.component';
import { MapsComponent }   		from './maps.component';
import { GrapheMapComponent }   from './graphe-map.component';

import { TreeViewComponent }	from './tree-view.component';

import { MapService }			from './map.service';
import { PlayerService }		from './player.service';


@NgModule({
  imports:      [ BrowserModule, HttpModule, routing ],

  declarations: [ DashboardComponent, PlayersComponent, MapsComponent, GrapheMapComponent, TreeViewComponent ],
  providers: 	[ MapService, PlayerService ],
  bootstrap:    [ DashboardComponent ]
})

export class AppModule { }
