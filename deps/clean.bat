@echo off

del /Q /S  "*.def" > nul 2>&1
del /Q /S  "*.def.in" > nul 2>&1
rmdir /S /Q "lib"  > nul 2>&1
