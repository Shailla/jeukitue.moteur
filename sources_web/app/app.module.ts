import { NgModule }      from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpModule }    from '@angular/http';

import { routing } 				from './app.routing';
import { DashboardComponent }   from './dashboard.component';
import { PlayersComponent }   	from './players.component';

@NgModule({
  imports:      [ BrowserModule, HttpModule, routing ],

  declarations: [ DashboardComponent, PlayersComponent ],
  providers: 	[],
  bootstrap:    [ DashboardComponent ]
})

export class AppModule { }
