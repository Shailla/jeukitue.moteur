rd deploy2\web\js /Q /S
mkdir deploy2\web\js
xcopy sources_web\*.js deploy2\web\js /S
xcopy sources_web\*.ts deploy2\web\js /S

rd deploy2\web\image /Q /S
mkdir deploy2\web\image
xcopy sources_web\*.jpg deploy2\web\image /S
xcopy sources_web\*.jpeg deploy2\web\image /S

rd deploy2\web\html /Q /S
mkdir deploy2\web\html
xcopy sources_web\*.html deploy2\web\html /S

rd deploy2\web\json /Q /S
mkdir deploy2\web\json
xcopy sources_web\*.json deploy2\web\json /S

rd deploy2\web\css /Q /S
mkdir deploy2\web\css
xcopy sources_web\*.css deploy2\web\css /S