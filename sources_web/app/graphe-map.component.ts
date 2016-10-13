import { Component, OnInit } from '@angular/core';

import { TreeViewComponent } from './tree-view.component';
import { TreeItem } from './treeItem';
import { Map } 		from './map';

@Component({
  selector: 	'my-graphe',
  template: 	`<h1 class="title" style="text-align: center;">Graphe Map courante</h1>
				<tree-view [items]="items"></tree-view>
				`
})
export class GrapheMapComponent implements OnInit {

	items: TreeItem[];
	
	constructor() {
		let item11 = new TreeItem("Item11",[]);
		let item12 = new TreeItem("Geo12",[]);
		item12.class = "geo";
		let item13 = new TreeItem("Item13",[]);
		let parent1 = new TreeItem("Parent1",[item11,item12,item13]);
		let item21 = new TreeItem("Item21",[]);
		let item22 = new TreeItem("Item22",[]);
		let item23 = new TreeItem("Item23",[]);
		let parent2 = new TreeItem("Parent2",[item21,item22,item23]);
		let item1 = new TreeItem("Item1",[]);
		let item2 = new TreeItem("Item2",[]);
		this.items = [parent1, parent2, item1, item2];
	}
	
	ngOnInit(): void {
	}
}