export class MapElement {
	// Data
	type: string;
	id: number; 
	name: string; 
	elements: Array<MapElement>;
	
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
		
		for(let iter of jsonObject.elements) {
			this.elements.push(new MapElement().fromJson(iter));
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
}