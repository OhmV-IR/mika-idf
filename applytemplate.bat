@echo off
setlocal enabledelayedexpansion

echo [INFO] Copying ui component
rmdir /s /q "components\ui\src"
del "components\ui\ui.h"
del "components\ui\library.properties"
robocopy "EXPORT_HERE\Player\libraries\ui\src" "components\ui\src" /E /IS /R:3 /W:5
copy "EXPORT_HERE\Player\libraries\ui\ui.h" "components\ui\ui.h"
copy "EXPORT_HERE\Player\libraries\library.properties" "components\ui\library.properties"
echo "[INFO] Done copying ui component"
echo "[INFO] When ready to exit monitor, press Ctrl and ] at the same time"
eim run "idf.py build flash monitor" v5.5.5