export class MapElement {
	name: string; 
	elements: Array<MapElement>;
	type: string;
    expanded: boolean;
    checked: boolean;
	
    constructor(name, elements, type) {
        this.name = name;
        this.elements = elements;
        this.type = type;
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