import { MapElement } 		from './MapElement';

export class MapGrapheElement {
	// Data
	type: string;
	id: number; 
	name: string; 
	elements: Array<MapGrapheElement>;
	
	hidden: boolean;
	highlighted: boolean;
	
	// Mise en forme
    expanded: boolean;
	checked: boolean;
	
	constructor() {       
		this.elements = [];
		
		this.hidden = false;
        this.highlighted = false;
		
		this.expanded = false;
		this.checked = false;
    }
	
	fromJson(jsonObject) {
		this.type = jsonObject.type;
		this.id = jsonObject.id;
		this.name = jsonObject.name;
		this.hidden = jsonObject.hidden;
		this.highlighted = jsonObject.highlighted;
		
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
		console.log("Simple click");
    }
		
	checkRecursive(state?: boolean) {
		let newState;
		
		if(state == undefined) {
			newState = !this.checked;
		}
		else {
			newState = state;
		}
	
		this.checked = newState;
        this.elements.forEach(d => {
            d.checkRecursive(newState);
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