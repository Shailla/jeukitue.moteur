export class MapElement {
	// Data
	type: string;
	id: number; 
	name: string;
	selected: boolean;
	
	constructor() {       
    }
	
	fromJson(jsonObject) {
		this.type = jsonObject.type;
		this.id = jsonObject.id;
		this.name = jsonObject.name;
		this.selected = jsonObject.selected;
		
		return this;
	}
}
