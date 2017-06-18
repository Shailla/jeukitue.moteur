import { EssaiPage } from './app.po';

describe('Jkt web', () => {
  let page: EssaiPage;

  beforeEach(() => {
    page = new EssaiPage();
  });

  it('should display welcome message', done => {
    page.navigateTo();
    page.getParagraphText()
      .then(msg => expect(msg).toEqual('Welcome to app!!'))
      .then(done, done.fail);
  });
});
