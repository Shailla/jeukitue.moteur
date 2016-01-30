System.register(['angular2/core', './players.service', 'angular2/http'], function(exports_1) {
    var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
        var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
        if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
        else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
        return c > 3 && r && Object.defineProperty(target, key, r), r;
    };
    var __metadata = (this && this.__metadata) || function (k, v) {
        if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(k, v);
    };
    var core_1, players_service_1, http_1;
    var PlayersListComponent;
    return {
        setters:[
            function (core_1_1) {
                core_1 = core_1_1;
            },
            function (players_service_1_1) {
                players_service_1 = players_service_1_1;
            },
            function (http_1_1) {
                http_1 = http_1_1;
            }],
        execute: function() {
            PlayersListComponent = (function () {
                function PlayersListComponent(_playersService) {
                    this._playersService = _playersService;
                }
                PlayersListComponent.prototype.ngOnInit = function () {
                    var _this = this;
                    this._playersService.getPlayers().subscribe(function (players) { return _this.players = players; }, console.error, function () { return console.log('Completed!'); });
                };
                PlayersListComponent = __decorate([
                    core_1.Component({
                        selector: 'players-list',
                        providers: [players_service_1.PlayersService, http_1.HTTP_BINDINGS],
                        templateUrl: 'app/players.list.component.html'
                    }), 
                    __metadata('design:paramtypes', [players_service_1.PlayersService])
                ], PlayersListComponent);
                return PlayersListComponent;
            })();
            exports_1("PlayersListComponent", PlayersListComponent);
        }
    }
});
//# sourceMappingURL=players.list.component.js.map