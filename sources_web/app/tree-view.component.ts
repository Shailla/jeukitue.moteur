import { Component, Input } from '@angular/core';

import { TreeItem } 		from './treeItem';

@Component({
	selector: 'tree-view',
	templateUrl: 'app/tree-view.component.html'
})
export class TreeViewComponent {
	@Input() items: Array<TreeItem>;
}