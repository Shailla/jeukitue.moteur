import { browser, by, element } from 'protractor';

export class EssaiPage {
  navigateTo() {
    return browser.get('/');
  }

  getParagraphText() {
    return element(by.css('app-dashboard h1')).getText();
  }
}
