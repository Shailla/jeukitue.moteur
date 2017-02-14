export class MapElement {
	// Data
	type: string;
	id: number; 
	name: string;
	highlighted: boolean;
	hidden: boolean;
	
	constructor() {       
    }
	
	fromJson(jsonObject) {
		this.type = jsonObject.type;
		this.id = jsonObject.id;
		this.name = jsonObject.name;
		this.highlighted = jsonObject.highlighted;
		this.hidden = jsonObject.hidden;
		
		return this;
	}
}
