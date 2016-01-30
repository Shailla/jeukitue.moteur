
rd js /Q /S
mkdir js
xcopy all_resources\*.js js /S
xcopy all_resources\*.ts js /S

rd image /Q /S
mkdir image
xcopy all_resources\*.jpg image /S
xcopy all_resources\*.jpeg image /S

rd html /Q /S
mkdir html
xcopy all_resources\*.html html /S

rd json /Q /S
mkdir json
xcopy all_resources\*.json json /S

rd css /Q /S
mkdir css
xcopy all_resources\*.css css /S