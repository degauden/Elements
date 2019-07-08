@echo off

if not "%ELEMENTS_NO_STRIP_PATH%" == "" goto :eof

set tmpfile="%TEMP%\StripPath_tmpsetup.bat"

python %ELEMENTS_PROJECT_ROOT%\cmake\scripts\PathStripper.py --shell=bat --output=%tmpfile% -e PATH -e PYTHONPATH -e ELEMENTS_AUX_PATH -e ELEMENTS_CONF_PATH -e HPATH

call %tmpfile%


if exist %tmpfile% del %tmpfile%
set tmpfile=

