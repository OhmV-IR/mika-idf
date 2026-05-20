@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
	    echo [ERROR] Missing origin directory!
	        echo Usage: %~0 "C:\Path\To\Origin"
		    exit /b 1
)

set "ORIGIN_DIR=%~1"

if not exist "%ORIGIN_DIR%" (
	echo [ERROR] Origin dir DNE: "%ORIGIN_DIR%"
	exit /b 1
)

echo [INFO] Copying ui component
robocopy "%ORIGIN_DIR%\Player\libraries\ui" "components\ui" /E /IS /R:3 /W:5
echo [INFO] Done copying ui component
