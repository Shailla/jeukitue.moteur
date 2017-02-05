import { MapElement } 		from './MapElement';

export class MapGrapheElement {
	// Data
	type: string;
	id: number; 
	name: string; 
	elements: Array<MapGrapheElement>;
	
	// Mise en forme
    expanded: boolean;
    checked: boolean;
	
	constructor() {       
		this.elements = [];
		this.expanded = false;
        this.checked = false;
    }
	
	fromJson(jsonObject) {
		this.type = jsonObject.type;
		this.id = jsonObject.id;
		this.name = jsonObject.name;
		this.checked = jsonObject.highlighted;
		
		for(let iter of jsonObject.elements) {
			this.elements.push(new MapGrapheElement().fromJson(iter));
		}
		
		return this;
	}

    toggle() {
        this.expanded = !this.expanded;
    }
	
    check() {
        let newState = !this.checked;
        this.checked = newState;
        this.checkRecursive(newState);
    }
	
    checkRecursive(state) {
        this.elements.forEach(d => {
            d.checked = state;
            d.checkRecursive(state);
        })
    }
	
	updateMapElement(mapElement: MapElement) {
        this.elements.forEach(element => {
            if(element.id == mapElement.id) {
				element.name = mapElement.name;
				element.checked = mapElement.highlighted;
			}
			
            element.updateMapElement(mapElement);
        })
    }
}