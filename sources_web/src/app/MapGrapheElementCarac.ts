export class MapGrapheElementCarac {
  // Data
  caracName: string;
  caracUpdatable: boolean;
  caracType: string;
  caracGroup: string;
  caracValue: any;

  constructor() {
  }

  fromJson(caracName: string, jsonObject: any) {
    this.caracName = caracName;
    this.caracUpdatable = jsonObject.caracUpdatable;
    this.caracType = jsonObject.caracType;
    this.caracGroup = jsonObject.caracGroup;
    this.caracValue = jsonObject.caracValue;

    return this;
  }
}
