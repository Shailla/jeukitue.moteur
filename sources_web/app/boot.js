System.register(['angular2/platform/browser', 'angular2/http', 'angular2/router', './maps.service', './players.service', './dashboard.component', './maps.list.component', './players.list.component'], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var browser_1, http_1, router_1, maps_service_1, players_service_1, dashboard_component_1, maps_list_component_1, players_list_component_1;
    return {
        setters:[
            function (browser_1_1) {
                browser_1 = browser_1_1;
            },
            function (http_1_1) {
                http_1 = http_1_1;
            },
            function (router_1_1) {
                router_1 = router_1_1;
            },
            function (maps_service_1_1) {
                maps_service_1 = maps_service_1_1;
            },
            function (players_service_1_1) {
                players_service_1 = players_service_1_1;
            },
            function (dashboard_component_1_1) {
                dashboard_component_1 = dashboard_component_1_1;
            },
            function (maps_list_component_1_1) {
                maps_list_component_1 = maps_list_component_1_1;
            },
            function (players_list_component_1_1) {
                players_list_component_1 = players_list_component_1_1;
            }],
        execute: function() {
            browser_1.bootstrap(dashboard_component_1.DashboardComponent, [router_1.ROUTER_PROVIDERS]).catch(function (err) { return console.error(err); });
            browser_1.bootstrap(maps_list_component_1.MapsListComponent, [maps_service_1.MapsService, http_1.HTTP_BINDINGS]).catch(function (err) { return console.error(err); });
            browser_1.bootstrap(players_list_component_1.PlayersListComponent, [players_service_1.PlayersService, http_1.HTTP_BINDINGS]).catch(function (err) { return console.error(err); });
        }
    }
});
//# sourceMappingURL=boot.js.map