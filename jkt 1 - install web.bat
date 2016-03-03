rd deploy\web\js /Q /S
mkdir deploy\web\js
xcopy sources_web\*.js deploy\web\js /S
xcopy sources_web\*.ts deploy\web\js /S

rd deploy\web\image /Q /S
mkdir deploy\web\image
xcopy sources_web\*.jpg deploy\web\image /S
xcopy sources_web\*.jpeg deploy\web\image /S

rd deploy\web\html /Q /S
mkdir deploy\web\html
xcopy sources_web\*.html deploy\web\html /S

rd deploy\web\json /Q /S
mkdir deploy\web\json
xcopy sources_web\*.json deploy\web\json /S

rd deploy\web\css /Q /S
mkdir deploy\web\css
xcopy sources_web\*.css deploy\web\css /S