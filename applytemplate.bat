@echo off
setlocal enabledelayedexpansion

if not exist "EXPORT_HERE" (
    echo "[ERROR] EXPORT_HERE folder DNE."
    exit /b 1
)

echo [INFO] Copying ui component
rmdir /s /q "components\ui\src"
xcopy "EXPORT_HERE\*" "components\ui\src\" /E /Y /I /Q
echo "[INFO] Done copying ui component"
echo "[INFO] When ready to exit monitor, press Ctrl and ] at the same time"
if "%~1"!="--no-flash" (
    eim run "idf.py reconfigure build flash monitor" v5.5.5
)