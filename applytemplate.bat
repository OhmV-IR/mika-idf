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
rmdir /s /q "components\ui\src"
del "components\ui\ui.h"
del "components\ui\library.properties"
robocopy "%ORIGIN_DIR%\Player\libraries\ui\src" "components\ui\src" /E /IS /R:3 /W:5
copy "%ORIGIN_DIR%\Player\libraries\ui\ui.h" "components\ui\ui.h"
copy "%ORIGIN_DIR%\Player\libraries\library.properties" "components\ui\library.properties"
echo [INFO] Done copying ui component
idf.py fullclean
