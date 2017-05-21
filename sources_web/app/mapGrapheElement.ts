import { MapElement } 					from './MapElement';

export class MapGrapheElement {
	// Data
	type: string;
	id: number; 
	name: string; 
	elements: Array<MapGrapheElement> = [];
	
	hidden: boolean = false;
	highlighted: boolean = false;
	
	// Mise en forme
    expanded: boolean = false;
	checked: boolean = false;
	
	constructor() {       
    }
	
	fromJson(jsonObject: any) {
		this.type = jsonObject.type;
		this.id = jsonObject.id;
		this.name = jsonObject.name;
		this.hidden = jsonObject.hidden;
		this.highlighted = jsonObject.highlighted;
		
		if(jsonObject.elements != undefined) {
			for(let iter of jsonObject.elements) {
				this.elements.push(new MapGrapheElement().fromJson(iter));
			}
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
		let newState: boolean;
		
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