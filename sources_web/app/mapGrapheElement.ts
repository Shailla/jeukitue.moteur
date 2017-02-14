import { MapElement } 		from './MapElement';

export class MapGrapheElement {
	// Data
	type: string;
	id: number; 
	name: string; 
	elements: Array<MapGrapheElement>;
	
	highlighted: boolean;
	hidden: boolean;
	
	// Mise en forme
    expanded: boolean;
	
	constructor() {       
		this.elements = [];
		
		this.hidden = false;
        this.highlighted = false;
		
		this.expanded = false;
    }
	
	fromJson(jsonObject) {
		this.type = jsonObject.type;
		this.id = jsonObject.id;
		this.name = jsonObject.name;
		this.highlighted = jsonObject.highlighted;
		this.hidden = jsonObject.hidden;
		
		for(let iter of jsonObject.elements) {
			this.elements.push(new MapGrapheElement().fromJson(iter));
		}
		
		return this;
	}

    toggle() {
        this.expanded = !this.expanded;
    }
	
    highlight() {
        let newState = !this.highlighted;
        this.highlighted = newState;
        this.highlightRecursive(newState);
    }
	
	hide() {
        let newState = !this.hidden;
        this.hidden = newState;
        this.hideRecursive(newState);
    }
	
    highlightRecursive(state) {
        this.elements.forEach(d => {
            d.highlighted = state;
            d.highlightRecursive(state);
        })
    }
	
	hideRecursive(state) {
        this.elements.forEach(d => {
            d.hidden = state;
            d.hideRecursive(state);
        })
    }
	
	updateMapElement(mapElement: MapElement) {
        this.elements.forEach(element => {
            if(element.id == mapElement.id) {
				element.name = mapElement.name;
				element.hidden = mapElement.hidden;
				element.highlighted = mapElement.highlighted;
			}
			
            element.updateMapElement(mapElement);
        })
    }
}