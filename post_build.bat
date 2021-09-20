@echo off
set PLUGIN_NAME=MidiChordRecognition
set OUT_DIR=..\out
set JS_BUILD_DIR=..\jsui\build\
set BUILD_DIR=build

cd /d %BUILD_DIR%
rmdir %OUT_DIR% /s/q
mkdir %OUT_DIR%
FOR /R %%A IN (%PLUGIN_NAME%.exe) DO copy "%%A" %OUT_DIR%
FOR /R %%A IN (%PLUGIN_NAME%.vst3) DO copy "%%A" %OUT_DIR%
robocopy %JS_BUILD_DIR% %OUT_DIR% /E