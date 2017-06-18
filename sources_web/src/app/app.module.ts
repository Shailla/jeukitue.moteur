import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpModule } from '@angular/http';
import { FormsModule } from '@angular/forms';

import { routing } from './app.routing';
import { DashboardComponent } from './dashboard.component';
import { PlayersComponent } from './players.component';
import { MapsComponent } from './maps.component';
import { GrapheMapComponent } from './graphe-map.component';
import { MapGrapheElementCaracComponent } from './map-element-carac-detail.component';
import { TreeViewComponent } from './tree-view.component';
import { MapElementDetailComponent }	from './map-element-detail.component';
import { MapControlsComponent } from './map-controls.component';

import { MapService }	from './map.service';
import { PlayerService } from './player.service';


@NgModule({
  imports:      [ BrowserModule, FormsModule, HttpModule, routing ],
  declarations: [ DashboardComponent, PlayersComponent, MapsComponent, GrapheMapComponent, MapGrapheElementCaracComponent, TreeViewComponent, MapElementDetailComponent, MapControlsComponent ],
  providers:    [ MapService, PlayerService ],
  bootstrap:    [ DashboardComponent ]
})

export class AppModule { }
