export class TreeItem {
	name: string; 
	items: Array<TreeItem>;
    expanded:boolean;
    checked:boolean;
	class:string;
	
    constructor(name,items) {
        this.name = name;
        this.items = items;
        this.expanded = false;
        this.checked = false;
		this.class = "";
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
        this.items.forEach(d => {
            d.checked = state;
            d.checkRecursive(state);
        })
    }
}