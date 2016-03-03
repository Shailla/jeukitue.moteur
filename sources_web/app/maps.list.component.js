System.register(['angular2/core', './maps.service', 'angular2/http'], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
        var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
        if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
        else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
        return c > 3 && r && Object.defineProperty(target, key, r), r;
    };
    var __metadata = (this && this.__metadata) || function (k, v) {
        if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(k, v);
    };
    var core_1, maps_service_1, http_1;
    var MapsListComponent;
    return {
        setters:[
            function (core_1_1) {
                core_1 = core_1_1;
            },
            function (maps_service_1_1) {
                maps_service_1 = maps_service_1_1;
            },
            function (http_1_1) {
                http_1 = http_1_1;
            }],
        execute: function() {
            MapsListComponent = (function () {
                function MapsListComponent(_mapsService) {
                    this._mapsService = _mapsService;
                }
                MapsListComponent.prototype.ngOnInit = function () {
                    var _this = this;
                    this._mapsService.getMaps().subscribe(function (maps) { return _this.maps = maps; }, console.error, function () { return console.log('Completed!'); });
                };
                MapsListComponent = __decorate([
                    core_1.Component({
                        selector: 'maps-list',
                        providers: [maps_service_1.MapsService, http_1.HTTP_BINDINGS],
                        templateUrl: 'app/maps.list.component.html'
                    }), 
                    __metadata('design:paramtypes', [maps_service_1.MapsService])
                ], MapsListComponent);
                return MapsListComponent;
            }());
            exports_1("MapsListComponent", MapsListComponent);
        }
    }
});
//# sourceMappingURL=maps.list.component.js.map