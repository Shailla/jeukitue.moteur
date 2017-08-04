import { MapGrapheElementCarac } from './MapGrapheElementCarac';

export class MapElement {
  // Data
  type: string;
  id: number;
  name: string;

  caracs: MapGrapheElementCarac[] = []

  constructor() {
  }

  fromJson(jsonObject: any) {
    this.type = jsonObject.type;
    this.id = jsonObject.id;
    this.name = jsonObject.name;

    console.log("essai 1" + JSON.stringify(jsonObject));

    if (jsonObject.caracs != undefined) {
      console.log("essai 2");

      //      jsonObject.caracs.foreach(carac => {
      //        console.log("coucou" + carac);
      //        this.caracs[carac.name] = new MapGrapheElementCarac().fromJson(carac);
      //      });


      for (let caracName of Object.keys(jsonObject.caracs)) {
        console.log("coucou : " + caracName);
        let element = new MapGrapheElementCarac().fromJson(caracName, jsonObject.caracs[caracName])
        this.caracs.push(element);
        console.log("coucou : OK");
      }

      console.log("essai 3");

      return this;
    }
  }
}
