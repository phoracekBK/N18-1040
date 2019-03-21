
@echo off
ForFiles /p "C:\stc\data_out\backup" /s /d -30 /c "cmd /c del @file"
