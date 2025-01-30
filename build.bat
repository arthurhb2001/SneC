@echo off

if not exist ".\x64\Debug" mkdir bin
if not exist "SneC++\lib" mkdir lib

if not exist "x64\Debug\SDL3.dll" (
    echo SDL files not found. Downloading...
    powershell -NoProfile -ExecutionPolicy Bypass -File download-sdl.ps1
    if errorlevel 1 (
        echo Error: Failed to download or extract SDL3 files.
        exit /b 1
    )
) else (
    echo SDL files already present in x64\Debug folder, skipping download.
)

if not exist "x64\Debug\SDL3.dll" (
    echo Error: SDL3.dll not found in x64\Debug directory.
    exit /b 1
)

set "VS_DEV_CMD=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VS_DEV_CMD%" (
    echo Error: Cannot find Visual Studio Developer Command Prompt at "%VS_DEV_CMD%".
    exit /b 1
)

call "%VS_DEV_CMD%"

setlocal
set INCLUDE_PATH=SneC++\include
set SDL_LIB=SneC++\lib\SDL3.lib

cl SneC++\src\*.cpp /std:c++17 /EHsc /I"%INCLUDE_PATH%" /Fe:"x64\Debug\SneC++.exe" /Fo:"x64\Debug\\" /link "%SDL_LIB%" /subsystem:console 
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build succeeded.
endlocal

