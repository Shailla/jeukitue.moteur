export class MapElement {
	// Data
	type: string;
	id: number; 
	name: string; 
	
	constructor() {       
    }
	
	fromJson(jsonObject) {
		this.type = jsonObject.type;
		this.id = jsonObject.id;
		this.name = jsonObject.name;
		
		return this;
	}
}