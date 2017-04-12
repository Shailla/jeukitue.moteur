import { MapGrapheElementCharacteristic } 	from './MapGrapheElementCharacteristic';

export class MapElement {
	// Data
	type: string;
	id: number; 
	name: string;
	highlighted: boolean;
	hidden: boolean;
	
	characteristics: Array<MapGrapheElementCharacteristic> = [];
	
	constructor() {       
    }
	
	fromJson(jsonObject) {
		this.type = jsonObject.type;
		this.id = jsonObject.id;
		this.name = jsonObject.name;
		this.highlighted = jsonObject.highlighted;
		this.hidden = jsonObject.hidden;
		
		if(jsonObject.characteristics != undefined) {
			for(let iter of jsonObject.characteristics) {
				this.characteristics.push(new MapGrapheElementCharacteristic().fromJson(iter));
			}
		}
		
		return this;
	}
}
