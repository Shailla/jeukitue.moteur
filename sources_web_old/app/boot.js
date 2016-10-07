"use strict";
var browser_1 = require('angular2/platform/browser');
var http_1 = require('angular2/http');
var router_1 = require('angular2/router');
// Services
var maps_service_1 = require('./maps.service');
var players_service_1 = require('./players.service');
// Components
var dashboard_component_1 = require('./dashboard.component');
var maps_list_component_1 = require('./maps.list.component');
var players_list_component_1 = require('./players.list.component');
browser_1.bootstrap(dashboard_component_1.DashboardComponent, [router_1.ROUTER_PROVIDERS]).catch(function (err) { return console.error(err); });
browser_1.bootstrap(maps_list_component_1.MapsListComponent, [maps_service_1.MapsService, http_1.HTTP_BINDINGS]).catch(function (err) { return console.error(err); });
browser_1.bootstrap(players_list_component_1.PlayersListComponent, [players_service_1.PlayersService, http_1.HTTP_BINDINGS]).catch(function (err) { return console.error(err); });
//# sourceMappingURL=boot.js.map