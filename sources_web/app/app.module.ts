import { NgModule }      from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpModule }    from '@angular/http';
import { FormsModule }   from '@angular/forms';

import { routing } 								from './app.routing';
import { DashboardComponent }   				from './dashboard.component';
import { PlayersComponent }   					from './players.component';
import { MapsComponent }   						from './maps.component';
import { GrapheMapComponent }   				from './graphe-map.component';
import { MapGrapheElementCharacteristicComponent }   from './map-element-characteristic-component';
import { TreeViewComponent }					from './tree-view.component';
import { MapElementDetail }						from './map-element-detail.component';
import { MapControls }							from './map-controls.component';

import { MapService }							from './map.service';
import { PlayerService }						from './player.service';


@NgModule({
  imports:      [ BrowserModule, FormsModule, HttpModule, routing ],

  declarations: [ DashboardComponent, PlayersComponent, MapsComponent, GrapheMapComponent, MapGrapheElementCharacteristicComponent, TreeViewComponent, MapElementDetail, MapControls ],
  providers: 	[ MapService, PlayerService ],
  bootstrap:    [ DashboardComponent ]
})

export class AppModule { }
