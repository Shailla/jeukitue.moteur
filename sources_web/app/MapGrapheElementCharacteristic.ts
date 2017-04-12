export class MapGrapheElementCharacteristic {
	// Data
	name: string;
	updatable: boolean;
	type: string;
	group: string;
	value: any;
		
	constructor() {       
    }
	
	fromJson(jsonObject) {
		this.name = jsonObject.characName;
		this.updatable = jsonObject.characUpdatable;
		this.type = jsonObject.characType;
		this.group = jsonObject.characGroup;
		this.value = jsonObject.characValue;
		
		return this;
	}
}