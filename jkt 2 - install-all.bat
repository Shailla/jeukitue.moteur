del "deploy2\*.dll"
copy "deploy\*.dll" "deploy2"

copy "deploy\Debug-g++\jkt.exe" "deploy2"
copy "deploy\Debug-g++\libjkt.dll.a" "deploy2"

rmdir /S /Q "deploy2\plugins"
mkdir "deploy2\plugins"
xcopy /E /F "deploy\plugins" "deploy2\plugins"

rmdir /S /Q "deploy2\Map"
mkdir "deploy2\Map"
xcopy /E /F "deploy\Map" "deploy2\Map"