@echo off

rem Compiler Flags
rem remove from compiler flags for older computers /Wv:18 -MT
set CommonCompilerFlags= /EHsc -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4456 -FC -Z7 /IC:\z\bin\SDL2-devel-2.0.10-VC\include /IC:\z\bin\SDL2_ttf-2.0.15-VC\include /IC:\z\bin\SDL2_image-2.0.5-VC\include /IC:\z\bin\SDL2_mixer-2.0.4-VC\include

rem Linker Flags
set CommonLinkerFlags= -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib SDL2.lib SDL2main.lib SDL2_image.lib SDL2_ttf.lib SDL2_mixer.lib /LIBPATH:C:\z\bin\SDL2-devel-2.0.10-VC\lib\x64 /LIBPATH:C:\z\bin\SDL2_ttf-2.0.15-VC\lib\x64 /LIBPATH:C:\z\bin\SDL2_image-2.0.5-VC\lib\x64 /LIBPATH:C:\z\bin\SDL2_mixer-2.0.4-VC\lib\x64

IF NOT EXIST build mkdir build
pushd build

IF EXIST *.exe del *.exe > NUL 2> NUL
IF EXIST *.pdb del *.pdb > NUL 2> NUL

rem  /SUBSYSTEM:WINDOW

cl %CommonCompilerFlags% ../test.cpp "c:/z/cpp/sdl remake/_MI/Window.cpp" "c:/z/cpp/sdl remake/_MI/LTexture.cpp" -Fmtest.map /LD /link %CommonLinkerFlags% -PDB:test_%date:~-4,4%%date:~-10,2%%date:~-7,2%_%time:~3,2%%time:~6,2%.pdb -EXPORT:GameUpdateAndRender
cl %CommonCompilerFlags% ../main.cpp "c:/z/cpp/sdl remake/_MI/Window.cpp" "c:/z/cpp/sdl remake/_MI/LTexture.cpp" -Fmmain.map /link %CommonLinkerFlags%

rem IF ERRORLEVEL 0 start "New Window" cmd /c main.exe popd IF NOT popd
rem main.exe

popd
