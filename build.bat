@echo off
mkdir build
cd /d build
for /F "delims=" %%i in ('dir /b') do (rmdir "%%i" /s/q || del "%%i" /s/q)
cmake -A x64 ../ && msbuild /p:Configuration=Release MidiChordRecognition.sln