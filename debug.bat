@echo off
cd /d build
for /F "delims=" %%i in ('dir /b') do (rmdir "%%i" /s/q || del "%%i" /s/q)
cmake -A x64 ../ && msbuild /p:Configuration=Debug MidiChordRecognition.sln