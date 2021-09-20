@echo off
set PLUGIN_NAME=MidiChordRecognition
set OUT_DIR=..\out
set JS_BUILD_DIR=..\jsui\build\
set BUILD_DIR=build

cd /d %BUILD_DIR%
rmdir %OUT_DIR% /s/q
mkdir %OUT_DIR%
robocopy %JS_BUILD_DIR% %OUT_DIR% /E
for /f "delims=" %%i in ('dir /s /b /a-d %PLUGIN_NAME%.exe') do copy /b "%%~i" "%OUT_DIR%"
for /f "delims=" %%i in ('dir /s /b /a-d %PLUGIN_NAME%.vst3') do copy /b "%%~i" "%OUT_DIR%"