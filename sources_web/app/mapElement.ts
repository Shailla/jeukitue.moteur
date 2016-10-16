export class MapElement {
	type: string;
	id: int; 
	name: string; 
	elements: Array<MapElement>;
	
    expanded: boolean;
    checked: boolean;
	
    constructor(type, id, name) {
        this.type = type;
		this.id = id;
		this.name = name;
        this.elements = [];
        
		this.expanded = false;
        this.checked = false;
    }
	
	constructor(type, id, name, elements) {
        this.type = type;
		this.id = id;
		this.name = name;
        this.elements = elements;
        
		this.expanded = false;
        this.checked = false;
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