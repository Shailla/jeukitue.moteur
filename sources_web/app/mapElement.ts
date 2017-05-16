import { MapGrapheElementCharacteristic } 	from './MapGrapheElementCharacteristic';

export class MapElement {
	// Data
	type: string;
	id: number; 
	name: string;
	highlighted: boolean;
	hidden: boolean;
	
	caracs: { [name:string]:MapGrapheElementCharacteristic; } = {}
	
	constructor() {       
    }
	
	fromJson(jsonObject) {
		this.type = jsonObject.type;
		this.id = jsonObject.id;
		this.name = jsonObject.name;
		this.highlighted = jsonObject.highlighted;
		this.hidden = jsonObject.hidden;
		
		if(jsonObject.caracs != undefined) {
			for(let iter of jsonObject.caracs) {
				console.log(iter);
				this.caracs[iter.value] = new MapGrapheElementCharacteristic().fromJson(iter);
			}
		}
		
		return this;
	}
}
