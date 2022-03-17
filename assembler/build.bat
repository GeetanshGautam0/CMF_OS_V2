@echo off
cls 
color f0

python C:\Users\geeta\AppData\Roaming\Python\Python38\Scripts\pyinstaller.exe -F -c assemble_and_build.py
if exist __pycache__ del __pycache__\* && del __pycache__
if exist build del build\* && del build
del *.spec
if exist dist goto delete
goto end

:delete
if exist ..\assemble_and_build.exe del ..\assemble_and_build.exe
move dist\assemble_and_build.exe ..\assemble_and_build.exe
del dist\* && del dist
rem cd back to the parent dir since the paths are relative to that dir
cd ..
start assemble_and_build.exe -c -r
goto end

:end
pause 
exit