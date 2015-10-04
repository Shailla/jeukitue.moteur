call "jkt 1 - install.bat"
call "jkt 2 - install.bat"

cd deploy
start "jkt1" jkt.exe

cd ..\deploy2
start "jkt2" jkt.exe

cd ..