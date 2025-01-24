@echo off

if not exist "bin" mkdir bin
if not exist "lib" mkdir lib

if not exist "bin\SDL3.dll" (
    echo SDL files not found. Downloading...
    powershell -NoProfile -ExecutionPolicy Bypass -File download-sdl.ps1
    if errorlevel 1 (
        echo Error: Failed to download or extract SDL3 files.
        exit /b 1
    )
) else (
    echo SDL files already present in bin folder, skipping download.
)

if not exist "bin\SDL3.dll" (
    echo Error: SDL3.dll not found in bin directory.
    exit /b 1
)

set "VS_DEV_CMD=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VS_DEV_CMD%" (
    echo Error: Cannot find Visual Studio Developer Command Prompt at "%VS_DEV_CMD%".
    exit /b 1
)

call "%VS_DEV_CMD%"

setlocal
set INCLUDE_PATH=include
set SDL_LIB=lib\SDL3.lib

cl src\main.cpp src\game.cpp /std:c++17 /EHsc /I"%INCLUDE_PATH%" /Fe:"bin\main.exe" /Fo:"bin\\" /link "%SDL_LIB%" /subsystem:console 
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build succeeded.
endlocal

