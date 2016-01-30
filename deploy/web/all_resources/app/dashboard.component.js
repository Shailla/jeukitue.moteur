System.register(['angular2/core', 'angular2/router', './maps.list.component', './players.list.component'], function(exports_1) {
    var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
        var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
        if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
        else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
        return c > 3 && r && Object.defineProperty(target, key, r), r;
    };
    var __metadata = (this && this.__metadata) || function (k, v) {
        if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(k, v);
    };
    var core_1, router_1, maps_list_component_1, players_list_component_1;
    var DashboardComponent;
    return {
        setters:[
            function (core_1_1) {
                core_1 = core_1_1;
            },
            function (router_1_1) {
                router_1 = router_1_1;
            },
            function (maps_list_component_1_1) {
                maps_list_component_1 = maps_list_component_1_1;
            },
            function (players_list_component_1_1) {
                players_list_component_1 = players_list_component_1_1;
            }],
        execute: function() {
            DashboardComponent = (function () {
                function DashboardComponent() {
                }
                DashboardComponent = __decorate([
                    core_1.Component({
                        selector: 'dashboard',
                        directives: [router_1.ROUTER_DIRECTIVES],
                        templateUrl: 'app/dashboard.component.html'
                    }),
                    router_1.RouteConfig([
                        { path: '/maps-list', name: 'MapsList', component: maps_list_component_1.MapsListComponent },
                        { path: '/players-list', name: 'PlayersList', component: players_list_component_1.PlayersListComponent }
                    ]), 
                    __metadata('design:paramtypes', [])
                ], DashboardComponent);
                return DashboardComponent;
            })();
            exports_1("DashboardComponent", DashboardComponent);
        }
    }
});
//# sourceMappingURL=dashboard.component.js.map