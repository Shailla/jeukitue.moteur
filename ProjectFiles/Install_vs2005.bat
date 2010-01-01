
ECHO Copie des fichiers de l'IDE
COPY VS2005\*.* ..\

ECHO Copie des DLL nécessaires à l'exécution
COPY ..\Libs\Agar-1.3.4\lib\ag_core.dll
COPY ..\Libs\Agar-1.3.4\lib\ag_gui.dll
COPY ..\Libs\fmodapi375win\api\fmod.dll
COPY ..\Libs\FreeType\lib\freetype.dll
COPY ..\Libs\Pre-built.2\lib\pthreadVC2.dll
COPY ..\Libs\SDL-1.2.14\lib\SDL.dll
COPY ..\Libs\SDL_image-1.2.10\lib\zlib1.dll
COPY ..\Libs\SDL_image-1.2.10\lib\SDL_image.dll
COPY ..\Libs\SDL_image-1.2.10\lib\libtiff-3.dll
COPY ..\Libs\SDL_image-1.2.10\lib\libpng12-0.dll
COPY ..\Libs\SDL_image-1.2.10\lib\jpeg.dll
COPY ..\Libs\SDL_net-1.2.7\lib\SDL_net.dll